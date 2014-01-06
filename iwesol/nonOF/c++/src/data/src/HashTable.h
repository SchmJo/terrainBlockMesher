/*---------------------------------------------------------------------------*\
                               |
  _____        _______ ____    | IWESOL: IWES Open Library
 |_ _\ \      / / ____/ ___|   |
  | | \ \ /\ / /|  _| \___ \   | Copyright: Fraunhofer Institute for Wind
  | |  \ V  V / | |___ ___) |  | Energy and Energy System Technology IWES
 |___|  \_/\_/  |_____|____/   |
                               | http://www.iwes.fraunhofer.de
                               |
-------------------------------------------------------------------------------
License
    This file is part of IWESOL.

    IWESOL is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    IWESOL is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with IWESOL.  If not, see <http://www.gnu.org/licenses/>.

Class

Description

SourceFiles

References

\*---------------------------------------------------------------------------*/

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

namespace jlib{

/**
 * @class HashTable
 * @brief A basic HashTable, based on STL map.
 *
 * Class Comp needs a method 'bool operator()(const Key & A, const Key & B) const'
 * ordering the keys.
 */
template < class Data, class Key, class Comp = std::less< Key > >
class HashTable{

public:

	/// Constructor.
	HashTable(){}

	/// Constructor.
	HashTable(
			const std::vector< Key > & keys,
			const std::vector< Data > & data
			);

	/// set an element
	inline void set(
			const Key & key,
			const Data & data
			);

	/// erase an element
	void erase(
			const Key & key
			);

	/// check if an element is present
	inline bool found(
			const Key & key
			) const { return m_map.count(key); }

	/// return the size
	inline unsigned int size() const { return m_map.size(); }

	/// check if empty
	inline bool empty() const { return m_map.empty(); }

	/// return the list of keys
	inline const std::vector< Key > & toc() const;

	/// returns the index of a key in the toc list
	inline int index(const Key & key) const;

	/// returns a key
	inline const Key & key(int i) const { return toc()[i]; }

	/// returns data
	inline const Data & data(int i) const { return (*this)[key(i)]; }

	/// checks if key is first in list
	inline bool isFirst(const Key & key) const { return index(key) == 0; }

	/// checks if key is last in list
	inline bool isLast(const Key & key) const { return index(key) == size() - 1; }

	/// returns first key
	inline const Key & firstKey() const { return toc().front(); }

	/// returns last key
	inline const Key & lastKey() const { return toc().back(); }

	/// returns the first element
	inline Data & firstData() { return (*this)[toc().front()]; }

	/// returns the first element
	inline const Data & firstData() const { return (*this)[toc().front()]; }

	/// returns the last element
	inline Data & lastData() { return (*this)[toc().back()]; }

	/// returns the last element
	inline const Data & lastData() const { return (*this)[toc().back()]; }

	/// returns key of first element >= given key
	inline const Key & firstGreaterEqualKey(const Key & key) const { return m_map.lower_bound(key)->first; }

	/// returns data of first element >= given key
	inline const Data & firstGreaterEqualKeyData(const Key & key) const { return m_map.lower_bound(key)->second; }

	/// returns index of first element >= given key
	inline int firstGreaterEqualKeyIndex(const Key & key) const;

	/// returns key of first element > given key
	inline const Key & firstGreaterKey(const Key & key) const { return m_map.upper_bound(key)->first; }

	/// returns data of first element > given key
	inline const Data & firstGreaterKeyData(const Key & key) const { return m_map.upper_bound(key)->second; }

	/// returns index of first element > given key
	inline int firstGreaterKeyIndex(const Key & key) const;

	/// returns key of last element <= given key
	inline const Key & lastLessEqualKey(const Key & key) const;

	/// returns Data of last element <= given key
	inline const Data & lastLessEqualKeyData(const Key & key) const;

	/// returns index of last element <= given key
	inline int lastLessEqualKeyIndex(const Key & key) const;

	/// returns key of last element < given key
	inline const Key & lastLessKey(const Key & key) const;

	/// returns Data of last element < given key
	inline const Data & lastLessKeyData(const Key & key) const;

	/// returns index of last element < given key
	inline int lastLessKeyIndex(const Key & key) const;

	/// clear content
	inline void clear() { m_map.clear(); m_toc.clear(); m_indexMap.clear(); }

	// returns an element
	inline Data & operator[](const Key & key);

	// returns an element
	inline const Data & operator[](const Key & key) const;


private:

	/// the underlying map
	std::map< Key, Data , Comp > m_map;

	/// the key list
	mutable std::vector< Key > m_toc;

	/// the order list
	mutable std::map< Key, int , Comp > m_indexMap;
};

template < class Data, class Key, class Comp >
HashTable< Data, Key, Comp >::HashTable(
			const std::vector< Key > & keys,
			const std::vector< Data > & data
			){
	for(unsigned int i = 0; i < keys.size(); i++){
		set(keys[i],data[i]);
	}
}

template < class Data, class Key, class Comp >
inline void HashTable< Data, Key, Comp >::set(
			const Key & key,
			const Data & data
			){
	m_map.insert
	(
			typename std::map< Key, Data, Comp >::value_type
			(
					key,
					data
			)
	);
}

template < class Data, class Key, class Comp >
inline Data & HashTable< Data, Key, Comp >::operator[](
		const Key & key
		){
	return m_map.find(key)->second;
}

template < class Data, class Key, class Comp >
inline const Data & HashTable< Data, Key, Comp >::operator[](
		const Key & key
		) const {
	return m_map.find(key)->second;
}

template < class Data, class Key, class Comp >
void HashTable< Data, Key, Comp >::erase(
			const Key & key
			) {
	m_map.erase(key);
	if(m_indexMap.count(key)){
		typename std::vector< Key >::iterator it = std::find(m_toc.begin(), m_toc.end(), key);
		while(++it != m_toc.end()){
			m_indexMap[*it]--;
		}
		m_indexMap.erase(key);
	}
	if(m_toc.size() > 0){
		m_toc.erase
		(
				std::find(m_toc.begin(), m_toc.end(), key)
		);
	}
}


template < class Data, class Key, class Comp >
inline const std::vector< Key > & HashTable< Data, Key, Comp >::toc() const{
	if(m_toc.size() == size()) return m_toc;
	m_toc.resize(size());
	int counter = 0;
	for(typename std::map< Key, Data, Comp >::const_iterator it = m_map.begin(); it != m_map.end(); ++it){
		  m_toc[counter] = it->first;
		  counter++;
	}
	return m_toc;
}

template < class Data, class Key, class Comp >
inline int HashTable< Data, Key, Comp >::index(
		const Key & key
		) const{
	if(m_indexMap.size() != size()) {
		m_indexMap.clear();
		int counter = 0;
		for(typename std::map< Key, Data, Comp >::const_iterator it = m_map.begin(); it != m_map.end(); ++it){
			  m_indexMap.insert
			  (
					  typename std::map< Key, int, Comp >::value_type
					  (
							  it->first,
							  counter
					  )
			  );
			  counter++;
		}
	}
	if(m_indexMap.count(key)){
		return m_indexMap[key];
	}
	return -1;
}

template < class Data, class Key, class Comp >
inline const Key & HashTable< Data, Key, Comp >::lastLessEqualKey(
		const Key & key
		) const{
	typename std::map< Key, Data, Comp >::const_iterator itg  = m_map.upper_bound(key);
	itg--;
	return itg->first;
}

template < class Data, class Key, class Comp >
inline const Data & HashTable< Data, Key, Comp >::lastLessEqualKeyData(
		const Key & key
		) const{
	typename std::map< Key, Data, Comp >::const_iterator itg  = m_map.upper_bound(key);
	itg--;
	return itg->second;
}

template < class Data, class Key, class Comp >
inline const Key & HashTable< Data, Key, Comp >::lastLessKey(
		const Key & key
		) const{
	typename std::map< Key, Data, Comp >::const_iterator itg  = m_map.lower_bound(key);
	itg--;
	return itg->first;
}

template < class Data, class Key, class Comp >
inline const Data & HashTable< Data, Key, Comp >::lastLessKeyData(
		const Key & key
		) const{
	typename std::map< Key, Data, Comp >::const_iterator itg  = m_map.lower_bound(key);
	itg--;
	return itg->second;
}

template < class Data, class Key, class Comp >
inline int HashTable< Data, Key, Comp >::firstGreaterEqualKeyIndex(
		const Key & key
		) const {
	typename std::map< Key, Data, Comp >::const_iterator it = m_map.lower_bound(key);
	if(it == m_map.end()) return -1;
	return index(it->first);
}

template < class Data, class Key, class Comp >
inline int HashTable< Data, Key, Comp >::firstGreaterKeyIndex(
		const Key & key
		) const {
	typename std::map< Key, Data, Comp >::const_iterator it = m_map.lower_bound(key);
	if(it == m_map.end()) return -1;
	return index(it->first);
}

template < class Data, class Key, class Comp >
inline int HashTable< Data, Key, Comp >::lastLessEqualKeyIndex(
		const Key & key
		) const {
	typename std::map< Key, Data, Comp >::const_iterator it  = m_map.upper_bound(key);
	if(it == m_map.begin()) return -1;
	it--;
	return index(it->first);
}

template < class Data, class Key, class Comp >
inline int HashTable< Data, Key, Comp >::lastLessKeyIndex(
		const Key & key
		) const {
	typename std::map< Key, Data, Comp >::const_iterator it  = m_map.lower_bound(key);
	if(it == m_map.begin()) return -1;
	it--;
	return index(it->first);
}

}

#endif /* HASHTABLE_H_ */
