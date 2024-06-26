#pragma once
#include"HashBucket.hpp"


namespace wxj {

	template< class K, class V, class Hash = HashFunc<K>>
	class MyunorderMap {
	public:
		
		struct Mapkoft {
			const K& operator()(const std::pair<K, V>& kv) {
				return kv.first;
			}
		};

		typedef typename myhash_bucket::hash_bucket<K, std::pair<K, V>, Mapkoft, Hash>::iterator map_iterator;

		map_iterator begin() {
			return _hb.begin();
		}
		map_iterator end() {
			return _hb.end();
		}
		map_iterator Find(const K& key) {
			return _hb.Find(key);
		}
		std::pair<map_iterator, bool> Insert(const std::pair<K, V>& val) {
			return _hb.Insert(val);
		}
		bool Erase(K& key) {
			return _hb.Erase(key);
		}

		V& operator [](K& key) {
			map_iterator it = _hb.Insert(std::make_pair(key,V())).first;
			return it->second;
		}
	private:
		myhash_bucket::hash_bucket<K, std::pair<K, V>, Mapkoft, Hash> _hb;

	};
}
