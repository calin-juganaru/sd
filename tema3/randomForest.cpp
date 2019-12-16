// copyright Luca Istrate, Andrei Medar
#include "randomForest.h"
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include "decisionTree.h"

using std::vector;
using std::pair;
using std::string;
using std::mt19937;

vector<vector<int>> get_random_samples(const vector<vector<int>>& samples,
                                       int num_to_return) {
    // Călin
    // Intoarce un vector de marime num_to_return cu elemente random,
    // diferite din samples

    std::random_device rd;
    mt19937 gen(rd());

    auto ret = samples;
    std::shuffle(begin(ret), end(ret), gen);
    ret.resize(num_to_return);

    return ret;
}

RandomForest::RandomForest(int num_trees, const vector<vector<int>> &samples)
    : num_trees(num_trees), images(samples) {}

void RandomForest::build() {
    // Aloca pentru fiecare Tree cate n / num_trees
    // Unde n e numarul total de teste de training
    // Apoi antreneaza fiecare tree cu testele alese
    assert(!images.empty());
    vector<vector<int>> random_samples;

    int data_size = images.size() / num_trees;

    for (int i = 0; i < num_trees; i++) {
        // cout << "Creating Tree nr: " << i << endl;
        random_samples = get_random_samples(images, data_size);

        // Construieste un Tree nou si il antreneaza
        trees.push_back(Node());
        trees[trees.size() - 1].train(random_samples);
    }
}

int RandomForest::predict(const vector<int>& image) {
    // Luissa
    // Va intoarce cea mai probabila prezicere pentru testul din argument
    // se va interoga fiecare Tree si se va considera raspunsul final ca
    // fiind cel majoritar

    auto frequency = vector<int>(10, 0);

    for (const auto& tree : trees)
    	++frequency[tree.predict(image)];

    auto maxf = 0, ret = 0;
    for (auto i  = 0; i < 10; ++i)
		if (frequency[i] > maxf) {
			maxf = frequency[i];
			ret = i;
		}

    return ret;
}
