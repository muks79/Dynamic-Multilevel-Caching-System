#include <iostream>
#include <unordered_map>
#include <list>
#include <memory>
#include <vector>
#include <climits>
#include <mutex>

using namespace std;

class EvictionPolicy
{
public:
  virtual void access(const string &key) = 0;
  virtual string evict() = 0;
  virtual ~EvictionPolicy() = default;
};

class LRUEvictionPolicy : public EvictionPolicy
{
private:
  list<string> accessOrder;
  unordered_map<string, list<string>::iterator> cacheMap;
  mutex mtx;

public:
  void access(const string &key) override
  {
    lock_guard<mutex> lock(mtx);
    if (cacheMap.find(key) != cacheMap.end())
    {
      accessOrder.erase(cacheMap[key]);
    }
    accessOrder.push_back(key);
    cacheMap[key] = --accessOrder.end();
  }
  string evict() override
  {
    lock_guard<mutex> lock(mtx);
    string leastRecent = accessOrder.front();
    accessOrder.pop_front();
    cacheMap.erase(leastRecent);
    return leastRecent;
  }
};

class LFUEvictionPolicy : public EvictionPolicy
{
private:
  unordered_map<string, int> frequencyMap;
  unordered_map<string, int> keyOrder;
  int currentTime = 0;
  mutex mtx;

public:
  void access(const string &key) override
  {
    lock_guard<mutex> lock(mtx);
    frequencyMap[key]++;
    keyOrder[key] = currentTime++;
  }
  string evict() override
  {
    lock_guard<mutex> lock(mtx);
    string lfuKey;
    int minFreq = INT_MAX;
    int minOrder = INT_MAX;
    for (const auto &pair : frequencyMap)
    {
      const string &key = pair.first;
      int freq = pair.second;
      int order = keyOrder[key];
      if (freq < minFreq || (freq == minFreq && order < minOrder))
      {
        minFreq = freq;
        minOrder = order;
        lfuKey = key;
      }
    }
    frequencyMap.erase(lfuKey);
    keyOrder.erase(lfuKey);
    return lfuKey;
  }
};

class CacheLevel
{
private:
  int size;
  unordered_map<string, string> data;
  unique_ptr<EvictionPolicy> evictionPolicy;
  mutex mtx;

public:
  CacheLevel(int size, unique_ptr<EvictionPolicy> evictionPolicy)
      : size(size), evictionPolicy(move(evictionPolicy)) {}
  string get(const string &key)
  {
    lock_guard<mutex> lock(mtx);
    if (data.find(key) != data.end())
    {
      evictionPolicy->access(key);
      return data[key];
    }
    return "";
  }
  void put(const string &key, const string &value)
  {
    lock_guard<mutex> lock(mtx);
    if (data.size() >= size)
    {
      string evicted = evictionPolicy->evict();
      data.erase(evicted);
    }
    data[key] = value;
    evictionPolicy->access(key);
  }
  void display()
  {
    lock_guard<mutex> lock(mtx);
    for (const auto &pair : data)
    {
      cout << pair.first << ": " << pair.second << " ";
    }
    cout << endl;
  }
  bool contains(const string &key)
  {
    lock_guard<mutex> lock(mtx);
    return data.find(key) != data.end();
  }
  void update(const string &key, const string &value)
  {
    lock_guard<mutex> lock(mtx);
    data[key] = value;
  }
};

class MultilevelCacheSystem
{
private:
  vector<unique_ptr<CacheLevel>> cacheLevels;
  mutex mtx;

public:
  void addCacheLevel(int size, const string &evictionPolicy)
  {
    lock_guard<mutex> lock(mtx);
    unique_ptr<EvictionPolicy> policy;
    if (evictionPolicy == "LRU")
    {
      policy = make_unique<LRUEvictionPolicy>();
    }
    else if (evictionPolicy == "LFU")
    {
      policy = make_unique<LFUEvictionPolicy>();
    }
    cacheLevels.push_back(make_unique<CacheLevel>(size, move(policy)));
  }
  void removeCacheLevel(int level)
  {
    lock_guard<mutex> lock(mtx);
    if (level >= 1 && level <= cacheLevels.size())
    {
      cacheLevels.erase(cacheLevels.begin() + level - 1);
    }
  }
  string get(const string &key)
  {
    lock_guard<mutex> lock(mtx);
    for (int i = 0; i < cacheLevels.size(); ++i)
    {
      string value = cacheLevels[i]->get(key);
      if (!value.empty())
      {
        for (int j = i; j > 0; --j)
        {
          cacheLevels[j - 1]->update(key, value);
        }
        return value;
      }
    }
    return "";
  }
  void put(const string &key, const string &value)
  {
    lock_guard<mutex> lock(mtx);
    cacheLevels[0]->put(key, value);
  }
  void displayCache()
  {
    lock_guard<mutex> lock(mtx);
    int level = 1;
    for (const auto &cache : cacheLevels)
    {
      cout << "L" << level++ << " Cache: ";
      cache->display();
    }
  }
};

int main()
{
  MultilevelCacheSystem cacheSystem;
  cacheSystem.addCacheLevel(3, "LRU");
  cacheSystem.addCacheLevel(2, "LFU");

  cacheSystem.put("A", "1");
  cacheSystem.put("B", "2");
  cacheSystem.put("C", "3");

  cout << cacheSystem.get("A") << endl;
  cacheSystem.put("D", "4");

  cout << cacheSystem.get("C") << endl;

  cacheSystem.displayCache();

  return 0;
}
