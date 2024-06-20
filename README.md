# This repository has been migrated to the self-hosted ari-web Forgejo instance: <https://git.ari.lt/ari/libtrie>
# Libtrie

> Trie implementation in C89 for generating text.

See `src/main.c` for example usage. You may compile it using `make`:

```sh
make -j$(nproc --all)
LD_LIBRARY_PATH="$(pwd)" ./trie
```

Licensing: Public domain

To test simply run `make test` :)
