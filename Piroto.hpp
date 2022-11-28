/* find(string, int)
-> buscar el string, después el int AQUI

findRange(string, int, int)
-> buscar el string, después el range AQUI */
#include "PatriciaTrie.hpp"

class Piroto {
 private:
  PatriciaTrie* Patricia;

 public:
  Piroto() { Patricia = new PatriciaTrie; }

  void insert(const string& stringValue, int intValue, void* Record) {
    Patricia->insert(stringValue, intValue, Record);
  }

  void* exactSearch(const string& stringValue, int intValue) {
    BPlusTree<3>* BPlusTree = Patricia->find(stringValue);
    if (BPlusTree == nullptr)
      return nullptr;
    return BPlusTree->search(intValue);
  }

  vector<void*> rangeSearch(const string& stringValue,
                            LowerBound lowerSearch,
                            int lowerBound,
                            UpperBound upperSearch,
                            int upperBound) {
    BPlusTree<3>* BPlusTree = Patricia->find(stringValue);
    if (BPlusTree == nullptr)
      return {};
    return BPlusTree->range(lowerSearch, lowerBound, upperSearch, upperBound);
  }

  vector<void*> rangeSearch(string stringValue,
                            SearchCriteria criteria,
                            int bound) {
    BPlusTree<3>* BPlusTree = Patricia->find(stringValue);
    if (BPlusTree == nullptr)
      return {};
    return BPlusTree->range(criteria, bound);
  }

  vector<void*> doubleRangeSearch() { return {}; }

  void print() { Patricia->print(); }
};