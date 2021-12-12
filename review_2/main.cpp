#include <iostream>
#include <vector>
#include <algorithm>

const char POSITIVE_ANSWER[] = "YES";
const char NEGATIVE_ANSWER[] = "NO";

struct Query {
    int firstBox;
    int secondBox;
    int difference;
};

class DisjointSetUnion {
public:
    explicit DisjointSetUnion(const size_t size);
    bool unionSets(const Query& query);
    size_t size() const;
    int getDifference(const int index);

private:
    struct Set {
        Set* parent;
        int rank;
        int differenceWithMinimum;
        int differenceWithParent;
        bool isStart;
    };

    std::vector<Set> sets;

    void makeSet(const int index);
    Set* findSet(Set* set);
    bool merge(Set* firstSet, Set* secondSet, const int difference);
};

struct CoinsDistributionData {
    DisjointSetUnion boxes;
    std::vector<Query> notes;
};

struct DistributionPossibilityResult {
    bool isPossible;
    uint64_t notPossibleAfter;
    std::vector<int> result;
};

DisjointSetUnion::DisjointSetUnion(const size_t size) {
    sets = std::vector<Set>(size);
    for (int i = 0; i < size; i++) {
        makeSet(i);
    }
    sets[0].isStart = true;
}

bool DisjointSetUnion::unionSets(const Query& query) {
    Set* firstSetParent = findSet(&sets[query.firstBox]);
    Set* secondSetParent = findSet(&sets[query.secondBox]);
    if (firstSetParent == secondSetParent &&
        sets[query.secondBox].differenceWithParent + query.difference !=
        sets[query.firstBox].differenceWithParent) {
        return false;
    }

    int difference = query.difference +
                     sets[query.secondBox].differenceWithParent -
                     sets[query.firstBox].differenceWithParent;
    return merge(firstSetParent, secondSetParent, difference);
}

size_t DisjointSetUnion::size() const {
    return sets.size();
}

int DisjointSetUnion::getDifference(const int index) {
    return findSet(&sets.at(index))->differenceWithMinimum -
           sets.at(index).differenceWithParent;
}

void DisjointSetUnion::makeSet(const int index) {
    Set* temp = &sets[index];
    temp->parent = &sets[index];
    temp->rank = 0;
    temp->differenceWithMinimum = 0;
    temp->differenceWithParent = 0;
    temp->isStart = false;
}

DisjointSetUnion::Set* DisjointSetUnion::findSet(Set* set) {
    if (set != set->parent) {
        Set* parent = findSet(set->parent);
        set->differenceWithParent += set->parent->differenceWithParent;
        set->parent = parent;
    }
    return set->parent;
}

bool DisjointSetUnion::merge(Set* firstSet, Set* secondSet, int difference) {
    if (firstSet->rank > secondSet->rank) {
        std::swap(firstSet, secondSet);
        difference = -difference;
    }

    firstSet->parent = secondSet;
    if (firstSet->rank == secondSet->rank) {
        secondSet->rank++;
    }
    firstSet->differenceWithParent = difference;
    secondSet->differenceWithMinimum =
            std::max(secondSet->differenceWithMinimum,
                     difference + firstSet->differenceWithMinimum);

    if (firstSet->isStart) {
        secondSet->isStart = true;
    }

    if (secondSet->isStart) {
        Set* first = &sets[0];
        findSet(first);
        if (first->differenceWithParent -
            first->parent->differenceWithMinimum < 0) {
            return false;
        }
    }
    return true;
}

CoinsDistributionData readCoinsDistributionData(std::istream& istream) {
    int boxCount;
    int queriesCount;
    istream >> boxCount >> queriesCount;
    CoinsDistributionData coinsDistributionData =
        {DisjointSetUnion(boxCount),
        std::vector<Query>(queriesCount)};
    for (int i = 0; i < queriesCount; i++) {
        int firstBox, secondBox, difference;
        istream >> firstBox >> secondBox >> difference;
        coinsDistributionData.notes[i] = {firstBox, secondBox, difference};
    }
    return coinsDistributionData;
}

DistributionPossibilityResult getDistributionPossibility(
        const CoinsDistributionData& coinsDistributionData) {
    std::vector<Query> queries = coinsDistributionData.notes;
    DisjointSetUnion boxes = coinsDistributionData.boxes;
    for (size_t i = 0; i < queries.size(); i++) {
        if (!boxes.unionSets({queries[i].secondBox,
                              queries[i].firstBox, queries[i].difference})) {
            return {false, i + 1};
        }
    }

    DistributionPossibilityResult possibilityResult{true, 0,
        std::vector<int>(boxes.size())};
    for (size_t i = 0; i < boxes.size(); i++) {
        possibilityResult.result[i] = boxes.getDifference(i);
    }
    return possibilityResult;
}

void printPossibilityResult(std::ostream& ostream,
    const DistributionPossibilityResult& possibilityResult) {
    if (possibilityResult.isPossible) {
        ostream << POSITIVE_ANSWER << "\n";
        for (int i : possibilityResult.result) {
            ostream << i << " ";
        }
        ostream << "\n";
    } else {
        ostream << NEGATIVE_ANSWER << "\n";
        ostream << possibilityResult.notPossibleAfter << "\n";
    }
}

int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    const CoinsDistributionData coinsDistributionData = readCoinsDistributionData(std::cin);
    const DistributionPossibilityResult possibilityResult =
            getDistributionPossibility(coinsDistributionData);
    printPossibilityResult(std::cout, possibilityResult);
    return 0;
}
