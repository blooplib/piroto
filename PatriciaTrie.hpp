#include <algorithm>
#include <iostream>
#include <unordered_map>
#include "BPlusTree.cpp"

using namespace std;

class Character {
 private:
  string terminal;
  int size;

  friend class PatriciaNode;

 public:
  Character(const string& terminal)
      : terminal(terminal), size(terminal.length()) {}
  bool equals(const string& value) {
    if (size != value.length())
      return false;
    for (int i = 0; i <= terminal.length(); i++) {
      if (terminal[i] != value[i])
        return false;
    }
    return true;
  }

  int matchSize(const string& value) {
    int minLength = min(size, (int)value.length());
    int i = 0;
    for (; i < minLength; i++) {
      if (terminal[i] != value[i])
        return i;
    }
    return i;
  }

  void cut(int max) {
    terminal = terminal.substr(0, max);
    size = terminal.length();
  }
};

class PatriciaNode {
 private:
  unordered_map<char, PatriciaNode*>* letters;
  bool leaf;
  Character* word;
  BPlusTree<3>* BPlus;

  string cut(const string& value, int min, int max) {
    return value.substr(min, max);
  }

  void addKey(const string& value, int data, void* Record) {
    (*letters)[value[0]] = new PatriciaNode(value, data, Record);
  }

  void addKey(const string& value, BPlusTree<3>* data) {
    (*letters)[value[0]] = new PatriciaNode(value, data);
  }

  void searchNext(const string& value, int data, void* Record) {
    auto next = letters->find(value[0]);
    if (next == letters->end())
      addKey(value, data, Record);
    else
      next->second->insert(value, data, Record);
  }

  void insertHere(int data, void* Record) {
    if (!leaf)
      leaf = true;
    if (BPlus == nullptr)
      BPlus = new BPlusTree<3>();
    BPlus->insert(data, Record);
  }

  void split(const string& value, int data, void* Record, int matchSize) {
    // hoja
    if (letters->size() == 0) {
      // se crea 1 hoja
      addKey(cut(word->terminal, matchSize, word->size), BPlus);
      BPlus = nullptr;
      // se inserta aquí después del split
      if (matchSize == value.length())
        insertHere(data, Record);
      // se hace split en 2 hojas
      else {
        leaf = false;
        addKey(cut(value, matchSize, value.length()), data, Record);
      }
    } else {
      string childWord = cut(word->terminal, matchSize, value.length());
      PatriciaNode* child = new PatriciaNode(childWord, BPlus, letters, leaf);
      letters = new unordered_map<char, PatriciaNode*>;
      (*letters)[childWord[0]] = child;
      BPlus = nullptr;
      // comparten una raíz y de ahí se abre en 2: el que se corto y el nuevo
      if (matchSize == value.length())
        insertHere(data, Record);
      // comparten una raíz y se crea una ramificación, el nuevo es la raíz
      else {
        leaf = false;
        addKey(cut(value, matchSize, value.length()), data, Record);
      }
    }
    word->cut(matchSize);
  }

 public:
  PatriciaNode(const string& value, int data, void* Record) : leaf(true) {
    word = new Character(value);
    letters = new unordered_map<char, PatriciaNode*>;
    BPlus = new BPlusTree<3>;
    BPlus->insert(data, Record);
  }

  PatriciaNode(const string& value, BPlusTree<3>* data) : leaf(true) {
    word = new Character(value);
    letters = new unordered_map<char, PatriciaNode*>;
    BPlus = data;
  }

  PatriciaNode(const string& value,
               BPlusTree<3>* data,
               unordered_map<char, PatriciaNode*>*& _letters,
               bool leaf)
      : leaf(leaf), BPlus(data) {
    word = new Character(value);
    letters = _letters;
  }

  void insert(const string& value, int data, void* Record) {
    int matchSize = word->matchSize(value);
    if (matchSize == word->size) {
      if (matchSize == value.length())
        insertHere(data, Record);
      else
        searchNext(cut(value, matchSize, value.length()), data, Record);
    } else
      split(value, data, Record, matchSize);
  }

  BPlusTree<3>* find(const string& value) {
    int matchSize = word->matchSize(value);
    if (leaf && matchSize == value.length()) {
      return BPlus;
    } else {
      auto next = letters->find(value[matchSize]);
      if (next != letters->end())
        return next->second->find(cut(value, matchSize, value.length()));
    }
    return nullptr;
  }

  void print(const string& s) {
    cout << s << word->terminal << " ";
    if (leaf)
      cout << "terminal";
    string b;
    if (BPlus)
      cout << BPlus->inorderString();
    cout << endl;
    for (auto node : (*letters)) {
      node.second->print(s + word->terminal);
    }
  }
};

class PatriciaTrie {
 private:
  unordered_map<char, PatriciaNode*> root;

 public:
  void insert(const string& value, int data, void* Record) {
    if (value.length() == 0)
      return;
    auto search = root.find(value[0]);
    if (search == root.end())
      root[value[0]] = new PatriciaNode(value, data, Record);
    else
      search->second->insert(value, data, Record);
  }

  BPlusTree<3>* find(const string& key) {
    auto search = root.find(key[0]);
    if (search == root.end())
      return nullptr;
    PatriciaNode* current = (*search).second;
    return current->find(key);
  }

  void print() {
    for (auto node : root) {
      node.second->print("");
      cout << endl;
    }
  }
};