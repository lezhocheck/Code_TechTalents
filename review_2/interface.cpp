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

struct BoxesAndNotes {
    DisjointSetUnion boxes;
    std::vector<Query> notes;
};

struct Answer {
    bool isPossible;
    uint64_t notPossibleAfter;
    std::vector<int> result;
};

BoxesAndNotes readInput(std::istream& istream);
Answer calculateAnswer(const BoxesAndNotes& boxesAndNotes);
void printResult(std::ostream& ostream, const Answer& answer);

int main() {
    std::cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    const BoxesAndNotes boxesAndNotes = readInput(std::cin);
    const Answer answer = calculateAnswer(boxesAndNotes);
    printResult(std::cout, answer);
    return 0;
}
