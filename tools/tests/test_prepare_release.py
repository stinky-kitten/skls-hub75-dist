"""Local finalizer regression tests. No commits, tags, publication or remote calls."""
import copy
import importlib.util
import json
import os
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
import zipfile

SCRIPT = Path(__file__).resolve().parents[1] / 'prepare_release.py'
spec = importlib.util.spec_from_file_location('finalizer', SCRIPT)
finalizer = importlib.util.module_from_spec(spec)
spec.loader.exec_module(finalizer)


class ReleasePolicyTests(unittest.TestCase):
    def test_version_and_asset_identity(self):
        version = '98.76.54-rehearsal.1+build'
        name, asset, tag, url = finalizer.identity(version)
        self.assertEqual(finalizer.numeric_version(version), '98.76.54')
        self.assertEqual(tag, 'v' + version)
        self.assertEqual(asset, name + '.zip')
        self.assertIn('/' + tag + '/' + asset, url)
        for invalid in ('', 'v1.2.3', '01.2.3', '1.2.3-01', '../1.2.3'):
            with self.subTest(version=invalid), self.assertRaises(ValueError):
                finalizer.identity(invalid)

    def test_historical_record_blocks_reuse_despite_preparation_status(self):
        version = finalizer.load_json((finalizer.ROOT / 'release.json').read_bytes())['version']
        with patch.object(finalizer, 'git_output', return_value=''):
            for candidate in (version, version + '+new-bytes'):
                with self.subTest(version=candidate), self.assertRaisesRegex(ValueError, 'reuse'):
                    finalizer.reject_used_version(candidate, True)

    def test_local_and_remote_tags_block_reuse(self):
        version = '98.76.54-rehearsal.1'
        with patch.object(finalizer, 'git_output', return_value='v' + version + '\n'):
            with self.assertRaisesRegex(ValueError, 'reuse'):
                finalizer.reject_used_version(version, True)
        with patch.object(finalizer, 'git_output', side_effect=['', 'abc\trefs/tags/v' + version + '^{}\n']):
            with self.assertRaisesRegex(ValueError, 'reuse'):
                finalizer.reject_used_version(version, False)

    def test_remote_failure_is_not_silently_treated_as_unused(self):
        with patch.object(finalizer, 'git_output', side_effect=['', ValueError('remote unavailable')]):
            with self.assertRaisesRegex(ValueError, 'remote unavailable'):
                finalizer.reject_used_version('98.76.54-rehearsal.1', False)

    def test_duplicate_json_keys_rejected(self):
        with self.assertRaisesRegex(ValueError, 'Duplicate JSON'):
            finalizer.load_json('{"version":"one","version":"two"}')


@unittest.skipUnless(os.environ.get('HUB75_TEST_CANDIDATE') and os.environ.get('HUB75_TEST_VALIDATION'),
                     'Supply a locally qualified candidate and its validation record for integration tests')
class CandidateTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.candidate = Path(os.environ['HUB75_TEST_CANDIDATE'])
        cls.report = finalizer.load_json(Path(os.environ['HUB75_TEST_VALIDATION']).read_bytes())
        cls.files = finalizer.read_candidate(cls.candidate)
        cls.version = cls.report['package_version']
        cls.swift = os.environ.get('SWIFT', 'swift')

    def test_complete_qualified_payload_accepted(self):
        finalizer.checked_payload(self.files, self.version, self.report)

    def test_each_missing_notice_rejected(self):
        for name in finalizer.NOTICES:
            files = dict(self.files)
            del files[name]
            with self.subTest(name=name), self.assertRaisesRegex(ValueError, 'inventory mismatch'):
                finalizer.checked_payload(files, self.version, self.report)

    def test_modified_payload_rejected(self):
        for name in self.files:
            files = dict(self.files)
            files[name] += b'\nchanged\n'
            with self.subTest(name=name), self.assertRaises((ValueError, json.JSONDecodeError)):
                finalizer.checked_payload(files, self.version, self.report)

    def test_unexpected_file_rejected(self):
        with self.assertRaisesRegex(ValueError, 'inventory mismatch'):
            finalizer.checked_payload(dict(self.files, unexpected=b'no'), self.version, self.report)

    def test_version_mismatch_rejected(self):
        with self.assertRaisesRegex(ValueError, 'version does not match'):
            finalizer.checked_payload(self.files, self.version + '+other', self.report)

    def test_rehashed_modification_does_not_reuse_validation_record(self):
        files = dict(self.files)
        manifest = finalizer.load_json(files['manifest.json'])
        name = 'LICENSES/PIO-PROVENANCE.md'
        files[name] += b'\nchanged\n'
        manifest['files'][name] = finalizer.digest(files[name])
        files['manifest.json'] = (json.dumps(manifest) + '\n').encode()
        with self.assertRaisesRegex(ValueError, 'record does not qualify'):
            finalizer.checked_payload(files, self.version, self.report)

    def test_failed_stale_and_mismatched_qualification_rejected(self):
        for key, value in (('passed', False), ('manifest_sha256', '0' * 64),
                           ('payload_file_count', 21), ('package_build_id', 'old'),
                           ('package_version', '1.2.3'), ('publication_status', 'published'),
                           ('checks', []), ('boards', {})):
            report = copy.deepcopy(self.report)
            report[key] = value
            with self.subTest(key=key), self.assertRaises(ValueError):
                finalizer.checked_payload(self.files, self.version, report)

    def test_missing_notice_cannot_be_hidden_by_rehashing_manifest(self):
        files = dict(self.files)
        name = 'LICENSES/PICO-EXAMPLES-BSD-3-Clause.txt'
        del files[name]
        manifest = finalizer.load_json(files['manifest.json'])
        del manifest['files'][name]
        files['manifest.json'] = json.dumps(manifest).encode()
        with self.assertRaisesRegex(ValueError, 'required licence/provenance'):
            finalizer.checked_payload(files, self.version, self.report)

    def test_symlink_candidate_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            link = Path(directory) / 'candidate'
            link.symlink_to(self.candidate, target_is_directory=True)
            with self.assertRaisesRegex(ValueError, 'regular directory'):
                finalizer.read_candidate(link)

    def test_rehearsal_preserves_every_byte_and_rejects_reuse_and_bad_checksums(self):
        with tempfile.TemporaryDirectory(prefix='hub75-finalizer-test-') as directory:
            output = Path(directory) / 'review'
            finalizer.prepare(self.candidate, output, self.version, self.report, self.swift, rehearsal=True)
            _, filename, _, _ = finalizer.identity(self.version)
            asset = output / filename
            with zipfile.ZipFile(asset) as archive:
                extracted = {name.split('/', 1)[1]: archive.read(name) for name in archive.namelist()}
            self.assertEqual(extracted, self.files)
            record = finalizer.load_json((output / 'release.json').read_bytes())
            self.assertTrue(record['rehearsal'])
            self.assertFalse(record['remote_tags_checked'])
            self.assertEqual(record['swiftpm']['kind'], 'declaration-only')
            before = asset.read_bytes()
            with self.assertRaises(FileExistsError):
                finalizer.prepare(self.candidate, output, self.version, self.report, self.swift, rehearsal=True)
            self.assertEqual(before, asset.read_bytes())
            asset.write_bytes(before + b'changed archive bytes')
            with self.assertRaisesRegex(ValueError, 'metadata/asset checksum'):
                finalizer.check(asset, self.version, self.report, self.swift)
            asset.write_bytes(before)
            original = (output / 'release.json').read_bytes()
            for key, value in (('asset_sha256', '0' * 64), ('tag', 'v1.2.3'),
                               ('asset_url', 'https://example.invalid/other.zip'),
                               ('manifest_sha256', '0' * 64)):
                broken = dict(record, **{key: value})
                (output / 'release.json').write_text(json.dumps(broken))
                with self.subTest(key=key), self.assertRaisesRegex(ValueError, 'metadata/asset checksum'):
                    finalizer.check(asset, self.version, self.report, self.swift)
            (output / 'release.json').write_bytes(original)
            (output / 'SHA256SUMS').write_text('0' * 64 + '  ' + filename + '\n')
            with self.assertRaisesRegex(ValueError, 'SHA256SUMS mismatch'):
                finalizer.check(asset, self.version, self.report, self.swift)


if __name__ == '__main__':
    unittest.main()
