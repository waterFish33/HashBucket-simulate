#pragma once
#include <vector>
#include <string>
#include <utility>
#include <cstddef>

template<class K>
struct HashFunc
{
	size_t operator()(const K& key)
	{
		return (size_t)key;
	}
};

template<>//对类型为string进行模板特化
struct HashFunc<std::string>
{
	size_t operator ()(const std::string& key)
	{
		// BKDR
		size_t hash = 0;
		for (auto e : key)
		{
			hash *= 31;
			hash += e;
		}
		return hash;
	}
};


namespace myhash_bucket {

	template<class T>
	struct Hash_Node {
		T _data;
		Hash_Node* _next;
		Hash_Node(const T& data)
			:_data(data), _next(nullptr) {}

	};

	//声明
	template<class K, class T, class koft, class Hash>
	class hash_bucket;

	//迭代器
	template <class K, class T, class Ptr, class Ref, class koft, class Hash >
	struct Iterator {
		typedef struct Hash_Node<T> Node;
		typedef struct Iterator<K, T, Ptr, Ref, koft, Hash > self;
		
		Node* _node;
		const hash_bucket< K, T, koft, Hash>* _hb;
		size_t _hashi;
	/*public:*/
		Iterator(Node* node, hash_bucket< K, T, koft, Hash>* hb, size_t hashi)
			:_node(node), _hb(hb), _hashi(hashi) {

		}
		Ref operator *() {
			return _node->_data;
		}
		Ptr operator ->() {
			return &_node->_data;
		}

		self& operator ++() {
			if (_node->_next) {
				_node = _node->_next;

			}
			else {
				_hashi++;
				while (_hashi<_hb->_tables.size()) {
					
					if (_hb->_tables[_hashi]) {
						_node = _hb->_tables[_hashi];
						break;
						
					}
					++_hashi;
					
				}
				if (_hashi == _hb->_tables.size()) {
					_node = nullptr;
					break;
				}
			}
			return *this;
		}

		bool operator !=(const self& s) {
			return _node != s._node;
		}
	};

	//哈希桶
	template<class K, class T, class koft, class Hash>
	class hash_bucket {
	public:
		template<class K, class T, class Ptr, class Ref, class koft, class Hash >
		friend struct Iterator;

		typedef struct Hash_Node<T> Node;
		typedef struct Iterator<K, T, T*, T&, koft, Hash> iterator;



	public:
		hash_bucket() {
			_tables.resize(10);
		}
		~hash_bucket() {
			for (size_t hashi = 0; hashi < _tables.size(); hashi++) {
				
				Node* cur = _tables[hashi];
				while (cur) {
					Node* next = cur->_next;
					
					delete cur;
					cur = next;

				}
				_tables[hashi] = nullptr;
			}
		}

		iterator begin() {

			for (size_t i = 0; i < _tables.size(); i++) {
				if (_tables[i] != nullptr) {
					return iterator(_tables[i], this, i);
				}

			}
			return end();
		}

		iterator end() {
			return iterator(nullptr, this, -1);
		}

		std::pair<iterator, bool> Insert(const T& data) {
			koft kot;
			Hash hash;
			//插入前进行判断，如果已经存在就插入失败
			iterator it = Find(kot(data));
			if (it != end()) {
				return std::make_pair(it, false);
			}
			//插入前对负载情况进行判定
			if (_size == _tables.size()) {
				std::vector<Node*> tmp;
				tmp.resize(2 * _tables.size(), nullptr);

				for (size_t i = 0; i < _tables.size(); i++) {
					Node* cur = _tables[i];

					while (_tables[i]) {
						Node* next = cur->_next;
						size_t hashi = hash(kot(cur->_data)) % tmp.size();

						cur->_next = tmp[hashi];
						tmp[hashi] = cur;

						cur = next;
					}
					_tables[i] = nullptr;
				}
				_tables.swap(tmp);
			}
			//插入新节点
			size_t hashi = hash(kot(data)) % _tables.size();
			
			Node* newnode = new Node(data);
			newnode->_next = _tables[hashi];
			_tables[hashi] = newnode;
			_size++;
			return std::make_pair(iterator(newnode, this, hashi), true);


		}
		bool Erase(const K& key) {
			koft kot;
			Hash hash;
			size_t hashi = hash(key) % _tables.size();
			Node* cur = _tables[hashi];
			Node* pre = nullptr;
			if (cur) {
				while (cur) {
					if (kot(cur->_data) == key) {
						if (pre == nullptr) {
							Node* next = cur->_next;
							delete cur;
							_tables[hashi] = next;
						}
						else {
							pre->_next = cur->_next;
							delete cur;
						}
						return true;
					}
					pre = cur;
					cur = cur->_next;
				}
			}

			return false;
		}

		iterator Find(const K& key) {
			koft kot;
			Hash hash;
			size_t hashi = hash(key) % _tables.size();
			Node* cur = _tables[hashi];
			while (cur) {
				if (kot(cur->_data) == key) {
					return iterator(cur, this, hashi);
				}
				cur = cur->_next;
			}
			return end();
		}
	private:
		std::vector<Node*> _tables;
		size_t _size;
	
	};
}