#pragma once
#include"HashBucket.hpp"
namespace wxj {
	using namespace myhash_bucket;
	template< class K, class Hash = HashFunc<K>>
	class MyunorderSet {
	public:
		
		struct Setkoft {
			const K& operator()(const K& k) {
				return k;
			}

		};
		typedef typename myhash_bucket::hash_bucket < K, K , Setkoft, Hash>::iterator set_iterator;
		set_iterator begin() {
			return _hb.begin();
		}
		set_iterator end() {
			return _hb.end();
		}
		set_iterator Find(const K& key) {
			return _hb.Find(key);
		}
		std::pair<set_iterator, bool> Insert(const K& key) {
			return _hb.Insert(key);
		}
		bool Erase(K& key) {
			return _hb.Erase(key);
		}
	private:
		myhash_bucket::hash_bucket<K, K, Setkoft, Hash> _hb;
	};
}
