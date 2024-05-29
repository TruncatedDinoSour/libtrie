#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "main.h"

#include "gen.h"
#include "rng.h"
#include "trie.h"

#define BUF_SIZE 1024

int main(const int argc, const char *const argv[]) {
    Trie *t;
    FILE *fp;
    int c, p;

    long min_size, count;

    uint64_t idx;
    uint8_t buf[BUF_SIZE] = {0};

    TrieRNG rng;
    uint8_t *sentence;
    uint64_t sentence_sz;

    if (argc < 6) {
        fprintf(stderr,
                "Usage: %s <model.bin> <dataset.txt> <seed> <min sentence "
                "size> <count>\n",
                argv[0]);
        fputs("- `model.bin` is a valid Libtrie model.\n"
              "- `dataset.txt` is your text with punctuation and whatnot. "
              "Newlines are ignored.\n"
              "- `seed` is any data, even text, the RNG will be seeded with.\n"
              "- `min sentence size` is the minimum generated sentence size.\n"
              "- `count` is the sentence count.\n",
              stderr);
        return 1;
    }

    min_size = atol(argv[4]);

    if (min_size < 1) {
        fprintf(stderr, "Invalid `min size`: %ld\n", min_size);
        return 1;
    }

    count = atol(argv[5]);

    if (count < 1) {
        fprintf(stderr, "Invalid `count`: %ld\n", count);
        return 1;
    }

    fp = fopen(argv[1], "rb");

    if (fp) {
        puts("Loading model...");
        t = trie_load_file(fp);

        if (!t) {
            fclose(fp);
            fputs("Failed to load tree\n", stderr);
            return 1;
        }
    } else {
        puts("Creating model...");

        fp = fopen(argv[2], "r");

        if (!fp) {
            fprintf(stderr, "Failing opening file %s: %s.\n", argv[1],
                    strerror(errno));
            return 1;
        }

        t = trie_create_node('\0');

        if (!t) {
            fclose(fp);
            fputs("Failed to create tree\n", stderr);
            return 1;
        }

        idx = 0;
        p   = 0;

        while ((c = fgetc(fp)) != EOF) {
            buf[idx++] =
                (c == '\n' || c == '\r') && p != '\n' ? ' ' : (uint8_t)c;

            if (idx >= BUF_SIZE - 1 || c == '.' || c == '?' || c == '!') {
                buf[idx] = '\0';
                trie_insert_sentence(t, buf);
                idx = 0;
            }

            p = c;
        }
    }

    fclose(fp);

    puts("Seeding RNG...");

    rng_seed(&rng, (const uint8_t *)argv[3], strlen(argv[3]));

    printf("Generating %ld random sentences...\n", count);

    for (idx = 0; idx < (uint64_t)count; ++idx) {
        rng_iter(rng.state, (uint64_t)count,
                 idx); /* Ensure randomness of different counts. */

        sentence = gen_trie_random(t, &rng, (uint64_t)min_size, &sentence_sz);

        if (!sentence)
            return 1;

        printf("[%lu,%lu] %s%s", idx, sentence_sz, sentence,
               ispunct(sentence[sentence_sz - 1]) ? " " : ". ");
    }

    putchar('\n');

    puts("Saving the tree...");

    fp = fopen(argv[1], "wb");

    if (!fp) {
        fputs("Failed to open the model file.\n", stderr);
        trie_free(t);
        return 1;
    }

    trie_save_file(fp, t);

    puts("Freeing the tree...");

    trie_free(t);

    return 0;
}
