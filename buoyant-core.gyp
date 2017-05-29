{
  "variables": {
    "gypkg_deps": [
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
    ],
  } ],
}
