// copyright Luca Istrate, Andrei Medar

#include "./decisionTree.h"  // NOLINT(build/include)
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::string;
using std::pair;
using std::vector;
using std::unordered_map;
using std::make_shared;

// structura unui nod din decision tree
// splitIndex = dimensiunea in functie de care se imparte
// split_value = valoarea in functie de care se imparte
// is_leaf si result sunt pentru cazul in care avem un nod frunza
Node::Node() {
    is_leaf = false;
    left = nullptr;
    right = nullptr;
}

void Node::make_decision_node(const int index, const int val) {
    split_index = index;
    split_value = val;
}

void Node::make_leaf(const vector<vector<int>>& samples,
                     const bool is_single_class) {
    // Călin
    // Seteaza nodul ca fiind de tip frunza (modificati is_leaf si result)
    // is_single_class = true -> toate testele au aceeasi clasa (acela e result)
    // is_single_class = false -> se alege clasa care apare cel mai des

    is_leaf = true;

    if (is_single_class) {
        result = samples.front().front();
    } else {
        auto frequency = vector<int>(10, 0);
        for (auto& image : samples)
            ++frequency[image.front()];

        auto maxf = 0;
        for (auto i = 0; i < 10; ++i)
            if (frequency[i] > maxf) {
                maxf = frequency[i];
                result = i;
            }
    }
}

pair<int, int> find_best_split(const vector<vector<int>>& samples,
                               const vector<int>& dimensions) {
    // Luissa
    // Intoarce cea mai buna dimensiune si valoare de split dintre testele
    // primite. Prin cel mai bun split (dimensiune si valoare)
    // ne referim la split-ul care maximizeaza IG
    // pair-ul intors este format din (split_index, split_value)

    auto splitValue = -1, splitIndex = -1, max_IG = -1;
    auto H_p = get_entropy(samples);

    for (const auto& i : dimensions) {
        //  alegem ca valoare de split o medie a valorilor
        //  de pe coloana curentă din vectorul dimensions.
        //  indexul de split va fi indexul acelei coloane
        auto uniq = compute_unique(samples, i);
        auto mean = std::accumulate(begin(uniq), end(uniq), 0);
        mean = pow(mean, 1.0 / uniq.size());

        //  facem split după cele 2 valori de mai sus
        auto aux = get_split_as_indexes(samples, i, mean);

        auto n_l = aux.first.size();
        auto n_r = aux.second.size();

        if (n_l && n_r) {
            //  cu valorile obținute în urma split-ului
            //  calculăm entropiile și IG-ul,
            //  apoi căutam maximul pentru IG
            auto H_l = get_entropy_by_indexes(samples, aux.first);
            auto H_r = get_entropy_by_indexes(samples, aux.second);

            auto IG = H_p - ((n_l * H_l) + (n_r * H_r)) / (n_l + n_r);

            if (IG > max_IG) {
                max_IG = IG;
                splitValue = mean;
                splitIndex = i;
            }
        }
    }

    return {splitIndex, splitValue};
}

void Node::train(const vector<vector<int>>& samples) {
    // Călin
    // Antreneaza nodul curent si copii sai, daca e nevoie
    // 1) verifica daca toate testele primite au aceeasi clasa (raspuns)
    // Daca da, acest nod devine frunza, altfel continua algoritmul.
    // 2) Daca nu exista niciun split valid, acest nod devine frunza. Altfel,
    // ia cel mai bun split si continua recursiv

    if (same_class(samples)) {
        make_leaf(samples, true);
    } else {
        //  căutăm cel mai bun split pentru sample-uri
        //  și dimensiunea fixă a oricărui vector (785)
        auto best_split = find_best_split(samples,
                            random_dimensions(785));

        if (best_split.first < 0 || best_split.second < 0) {
            make_leaf(samples, false);
        } else {
            //  dacă valorile de split găsite sunt valide
            //  facem split după ele,
            //  rădăcina devine nod de decizie
            //  și continuăm recursiv pe subarbori,
            //  pană dăm de frunze
            auto aux = split(samples, best_split.first,
                                      best_split.second);
            make_decision_node(best_split.first,
                               best_split.second);

            left = make_shared<Node>();
            left->train(aux.first);

            right = make_shared<Node>();
            right->train(aux.second);
        }
    }
}

int Node::predict(const vector<int>& image) const {
    // Luissa
    // Intoarce rezultatul prezis de catre decision tree
    //  (parcurgând recursiv arborele de decizie)

    if (is_leaf) {
		return result;
	} else {
	    if (image[split_index - 1] < split_value)
	    	return left->predict(image);
	    else
	    	return right->predict(image);
	}
}

bool same_class(const vector<vector<int>>& samples) {
    // Luissa
    // Verifica daca testele primite ca argument au toate aceeasi
    // clasa(rezultat). Este folosit in train pentru a determina daca
    // mai are rost sa caute split-uri

    auto first_class = samples.front().front();

    for (const auto& image : samples)
	   	if (image.front() != first_class)
	    		return false;

    return true;
}

float get_entropy(const vector<vector<int>> &samples) {
    // Intoarce entropia testelor primite
    assert(!samples.empty());
    vector<int> indexes;

    int size = samples.size();
    for (int i = 0; i < size; i++) indexes.push_back(i);

    return get_entropy_by_indexes(samples, indexes);
}

float get_entropy_by_indexes(const vector<vector<int>>& samples,
                             const vector<int>& index) {
    // Călin
    // Intoarce entropia subsetului din setul de teste total(samples)
    // Cu conditia ca subsetul sa contina testele ale caror indecsi se gasesc in
    // vectorul index (Se considera doar liniile din vectorul index)

    auto entropy = 0.0f;
    auto tests = samples.size();

    auto frequency = vector<int>(10, 0);
    for (const auto& image : samples)
        ++frequency[image.front()];

    for (const auto& freq : frequency)
        if (freq) {
            auto prob = (float) freq / tests;
            entropy -= prob * log2(prob);
        }

    return entropy;
}

vector<int> compute_unique(const vector<vector<int>>& samples, const int col) {
    // Luissa
    // Intoarce toate valorile (se elimina duplicatele)
    // care apar in setul de teste, pe coloana col

    auto size = samples.size();
    vector<int> uniqueValues, frequency(256, 0);

    //  folosim un vector de apariții
    //  pentru a elimina duplicatele
    for (const auto& image : samples)
    	++frequency[image[col]];

    for (auto i = 0; i < 256; ++i)
    	if (frequency[i] != 0)
    		uniqueValues.push_back(i);

    return uniqueValues;
}

pair<vector<vector<int>>, vector<vector<int>>> split(
    const vector<vector<int>> &samples, const int split_index,
    const int split_value) {
    // Intoarce cele 2 subseturi de teste obtinute in urma separarii
    // In functie de split_index si split_value
    vector<vector<int>> left, right;

    auto p = get_split_as_indexes(samples, split_index, split_value);
    for (const auto &i : p.first) left.push_back(samples[i]);
    for (const auto &i : p.second) right.push_back(samples[i]);

    return pair<vector<vector<int>>, vector<vector<int>>>(left, right);
}

pair<vector<int>, vector<int>> get_split_as_indexes(
        const vector<vector<int>>& samples,
        const int split_index,
        const int split_value) {
    // Călin
    // Intoarce indecsii sample-urilor din cele 2 subseturi obtinute in urma
    // separarii in functie de split_index si split_value

    vector<int> left, right;

    for (auto i = 0u; i < samples.size(); ++i) {
        if (samples[i][split_index] <= split_value)
            left.push_back(i);
        else
            right.push_back(i);
    }

    return {left, right};
}

vector<int> random_dimensions(const int size) {
    // Călin
    // Intoarce sqrt(size) dimensiuni diferite pe care sa caute splitul maxim
    // Precizare: Dimensiunile gasite sunt > 0 si < size

    size_t v_size = sqrt(size);
    auto rez = vector<int>(v_size);
    auto frequency = vector<bool>(size);

    std::generate(begin(rez), end(rez), [&] {
            static std::random_device rd;
            static std::mt19937 gen(rd());

            auto ret = (gen() % (size - 1)) + 1;

            //  caută o dimensiune care n-a
            //  mai fost generată pană acum
            while (frequency[ret])
                ret = (gen() % (size - 1)) + 1;

            frequency[ret] = true;

            return ret;
        });

	return rez;
}
