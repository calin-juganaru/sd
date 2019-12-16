// Copyright [2018] <Călin Jugănaru>

#ifndef HASH_H_
#define HASH_H_

#define in :

#include <string>
#include <vector>
#include <list>
#include <utility>

using std::pair;
using std::list;
using std::vector;
using std::string;
using std::to_string;

// -----------------------------------------------

size_t hash_func(size_t nr, size_t mod)
{
    return nr % mod;
}

size_t hash_func(string str, size_t mod)
{
    auto aux = 123456789u;
    for (auto& it : str)
        aux = (((aux << 1) + it) * 11 - 1) / 5;

    return aux % mod;
}

// -----------------------------------------------

template <typename K, typename V>
class hash_table
{
    vector<list<pair<K, V>>> bucket;
    size_t nr_buckets;

 public:
    //  trebuie inițializată tabela
    //      cu numărul de bucket-uri
    explicit hash_table(size_t n):
        nr_buckets(n)
        {
            bucket.resize(n);
        }

    void insert(pair<K, V> p)
    {
        auto ind = hash_func(p.first, nr_buckets);

        //  dacă nu este o coliziune,
        //      inserarea este simplă
        if (bucket[ind].empty())
        {
            bucket[ind].push_back(p);
            return;
        }
        else
        {
            for (auto& it in bucket[ind])
                if (it.first == p.first)
                {
                    it.second = p.second;
                    return;
                }

            //  dacă este coliziune, inserez
            //      la coada listei de la acel
            //      indice și păstrez cheia,
            //      pentru a nu se suprapune

            bucket[ind].push_back(p);
        }
    }

    //  dacă există sau nu o intrare
    //      cu cheia dată în tabelă
    bool find(K key)
    {
        auto ind = hash_func(key, nr_buckets);
        for (auto& it in bucket[ind])
            if (it.first == key)
                return true;

        return false;
    }

    //  operatorul de acces, pentru ușurință
    V& operator[](K key)
    {
        auto ind = hash_func(key, nr_buckets);
        for (auto& it in bucket[ind])
            if (it.first == key)
                return it.second;

        return bucket[ind].back().second;
    }
};

#endif  // HASH_H_
