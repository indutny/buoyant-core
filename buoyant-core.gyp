{
  "variables": {
    "gypkg_deps": [
    ],
    "gypkg_test_deps": [
      "git://github.com/indutny/mini-test.c.git@^1.0.0 => mini-test.gyp:mini-test",
    ],
  },

  "targets": [ {
    "target_name": "buoyant-core",
    "type": "<!(gypkg type)",

    "dependencies": [
      "<!@(gypkg deps <(gypkg_deps))",
    ],

    "direct_dependent_settings": {
      "include_dirs": [
        "include",
      ],
    },

    "include_dirs": [
      ".",
    ],

    "sources": [
      "src/buoyant-core.c",
      "src/emulator.c",
    ],
  }, {
    "target_name": "buoyant-core-test",
    "type": "executable",

    "dependencies": [
      "buoyant-core",
      "<!@(gypkg deps <(gypkg_test_deps))",
    ],

    "include_dirs": [
      ".",
    ],

    "sources": [
      "test/runner.c",
      "test/test-runtime-call.c",
    ],
  }],
}
