/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * ReferenceList.h                                                           *
 *                                                                             *
 * Copyright 2014 Tory Gaurnier <tory.gaurnier@linuxmail.org>                  *
 *                                                                             *
 * This program is free software; you can redistribute it and/or modify        *
 * it under the terms of the GNU Lesser General Public License as published by *
 * the Free Software Foundation; version 3.                                    *
 *                                                                             *
 * This program is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU Lesser General Public License for more details.                         *
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef REFERENCELIST_HPP
#define REFERENCELIST_HPP


#include <QDataStream>
#include <QDebug>
#include <QList>


/**
 * class ReferenceList
 *
 * Uses QList under the hood to store pointers, on the surface recieves and returns nothing but the
 * references.
 *
 * NOTE: Any method that calls it's QList<T*> counterpart with parameter T, T must not be const, if
 * it is then QList<T*> would have to be QList<const T *const> instead.
 */
template<class T>
class ReferenceList : public QList<T*> {
    public:
        // Forward declare iterators
        class const_iterator;
        class iterator;

        // Set iterators as friends
        friend class const_iterator;
        friend class iterator;


        ReferenceList() {}
        ReferenceList(const QList<T*> &other) : QList<T*>(other) {  }
        ReferenceList(const ReferenceList<T> &other) : QList<T*>(other) {  }
        ReferenceList(ReferenceList<T> &&other) : QList<T*>(std::move(other)) {  }
        ~ReferenceList() {}

        void append(T &value) {
            QList<T*>::append(&value);
        }

        void append(const ReferenceList<T> other) {
            QList<T*>::append(other);
        }

        const T & at(int i) const {
            return *QList<T*>::at(i);
        }

        T & back() {
            return *QList<T*>::back();
        }

        const T & back() const {
            return *QList<T*>::back();
        }

        iterator begin() {
            return iterator(iterator(QList<T*>::begin()));
        }

        const_iterator begin() const {
            return const_iterator(QList<T*>::begin());
        }

        const_iterator cbegin() const {
            return const_iterator(QList<T*>::cbegin());
        }

        const_iterator cend() const {
            return const_iterator(QList<T*>::cend());
        }

        void clear() {
            QList<T*>::clear();
        }

        const_iterator constBegin() const {
            return const_iterator(QList<T*>::constBegin());
        }

        const_iterator constEnd() const {
            return const_iterator(QList<T*>::constEnd());
        }

        bool contains(T &value) const {
            return QList<T*>::contains(&value);
        }

        int count(T &value) const {
            return QList<T*>::count(&value);
        }

        int count() const {
            return QList<T*>::count();
        }

        bool empty() const {
            return QList<T*>::empty();
        }

        iterator end() {
            return iterator(QList<T*>::end());
        }

        const_iterator end() const {
            return const_iterator(QList<T*>::end());
        }

        bool endsWith(T &value) const {
            return QList<T*>::endsWith(&value);
        }

        iterator erase(iterator pos) {
            return iterator(QList<T*>::erase(pos));
        }

        iterator erase(iterator begin, iterator end) {
            return iterator(QList<T*>::erase(begin, end));

        }

        T & first() {
            return *QList<T*>::first();
        }

        const T & first() const {
            return *QList<T*>::first();
        }

        /**
         * Inherited "from" methods are unsupported.
         */
        static ReferenceList<T> fromSet(const QSet<T> & set) = delete;
        static ReferenceList<T> fromStdList(const std::list<T> & list) = delete;
        static ReferenceList<T> fromVector(const QVector<T> & vector) = delete;

        T & front() {
            return *QList<T*>::front();
        }

        const T & front() const {
            return *QList<T*>::front();
        }

        int indexOf(T &value, int from = 0) const {
            return QList<T*>::indexOf(&value, from);
        }

        void insert(int i, T &value) {
            QList<T*>::insert(i, &value);
        }

        iterator insert(iterator before, T &value) {
            return iterator(QList<T*>::insert(before, &value));
        }

        bool isEmpty() const {
            return QList<T*>::isEmpty();
        }

        T & last() {
            return *QList<T*>::last();
        }

        const T & last() const {
            return *QList<T*>::last();
        }

        int lastIndexOf(T &value, int from = -1) const {
            return QList<T*>::lastIndexOf(&value, from);
        }

        int length() const {
            return QList<T*>::length();
        }

        ReferenceList<T> mid(int pos, int length = -1) const {
            return ReferenceList<T>(QList<T*>::mid(pos, length));
        }

        void move(int from, int to) {
            QList<T*>::move(from, to);
        }

        void pop_back() {
            QList<T*>::pop_back();
        }

        void pop_front() {
            QList<T*>::pop_front();
        }

        void prepend(T &value) {
            QList<T*>::prepend(&value);
        }

        void push_back(T &value) {
            QList<T*>::push_back(&value);
        }

        void push_front(T &value) {
            QList<T*>::push_front(&value);
        }

        int removeAll(T &value) {
            return QList<T*>::removeAll(&value);
        }

        void removeAt(int i) {
            QList<T*>::removeAt(i);
        }

        void removeFirst() {
            QList<T*>::removeFirst();
        }

        void removeLast() {
            QList<T*>::removeLast();
        }

        bool removeOne(T &value) {
            return QList<T*>::removeOne(&value);
        }

        void replace(int i, T &value) {
            QList<T*>::replace(i, &value);
        }

        void reserve(int alloc) {
            QList<T*>::reserve(alloc);
        }

        int size() const {
            return QList<T*>::size();
        }

        bool startsWith(T &value) const {
            return QList<T*>::startsWith(&value);
        }

        void swap(ReferenceList<T> &other) {
            QList<T*>::swap(other);
        }

        void swap(int i, int j) {
            QList<T*>::swap(i, j);
        }

        T & takeAt(int i) {
            return *QList<T*>::takeAt(i);
        }

        T & takeFirst() {
            return *QList<T*>::takeFirst();
        }

        T & takeLast() {
            return *QList<T*>::takeLast();
        }

        /**
         * Inherited "to" methods are not supported.
         */
        QSet<T> toSet() const = delete;
        std::list<T> toStdList() const = delete;
        QVector<T> toVector() const = delete;

        T & value(int i) const {
            return *QList<T*>::value(i);
        }

        T & value(int i, T &default_value) const {
            return *QList<T*>::value(i, &default_value);
        }

        bool operator!=(const ReferenceList<T> &other) const {
            return QList<T*>::operator!=(other);
        }

        ReferenceList<T> operator+(const ReferenceList<T> &other) const {
            return ReferenceList<T>(QList<T*>::operator+(other));
        }

        ReferenceList<T> & operator+=(const ReferenceList<T> &other) {
            QList<T*>::operator+=(other);
            return *this;
        }

        ReferenceList<T> & operator+=(T &value) {
            QList<T*>::operator+=(&value);
            return *this;
        }

        ReferenceList<T> & operator<<(const ReferenceList<T> &other) {
            QList<T*>::operator<<(other);
            return *this;
        }

        ReferenceList<T> & operator<<(T &value) {
            QList<T*>::operator<<(&value);
            return *this;
        }

        ReferenceList<T> & operator=(ReferenceList<T> &other) {
            QList<T*>::operator=(other);
            return *this;
        }

        ReferenceList & operator=(ReferenceList<T> &&other) {
            QList<T*>::operator=(std::move(other));
            return *this;
        }

        bool operator==(const ReferenceList<T> &other) const {
            return QList<T*>::operator==(other);
        }

        T & operator[](int i) {
            return *QList<T*>::operator[](i);
        }

        const T & operator[](int i) const {
            return *QList<T*>::operator[](i);
        }


        class iterator : public QList<T*>::iterator {
            public:
                iterator() {  }
                iterator(const typename QList<T*>::iterator &other)
                    : QList<T*>::iterator(other) {  }
                iterator(const iterator &other) : QList<T*>::iterator(other) {  }
                T & operator*() const { return *QList<T*>::iterator::operator*(); }
                T * operator->() const { return *QList<T*>::iterator::operator->(); }
                T & operator[](int j) const { return *QList<T*>::iterator::operator[](j); }
        };

        class const_iterator : public QList<T*>::const_iterator {
            public:
                const_iterator() {  }
                const_iterator(const typename QList<T*>::const_iterator &other)
                    : QList<T*>::const_iterator(other) {  }
                const_iterator(const const_iterator &other)
                    : QList<T*>::const_iterator(other) {  }
                const_iterator(const iterator &other)
                    : QList<T*>::const_iterator(other) {  }
                T & operator*() const { return *QList<T*>::const_iterator::operator*(); }
                T * operator->() const { return *QList<T*>::const_iterator::operator->(); }
                T & operator[](int j) const { return *QList<T*>::const_iterator::operator[](j); }
        };
};


/**
 * Implement QDebug << operator so that it will print out values rather than pointer addresses (do
 * to it implicitly converting to QList<T*>).
 */
template <class T>
QDebug operator<<(QDebug debug, const ReferenceList<T> &list) {
    debug.nospace() << '(';
    for(typename QList<T>::size_type i = 0; i < list.count(); ++i) {
        if(i) debug << ", ";
        debug << list.at(i);
    }

    debug << ')';

    return debug.space();
}


/**
* QDataStream << ReferenceList<T> should have same output as the QDataStream << QList<T>.
*/
template<class T>
QDataStream & operator<<(QDataStream &out, const ReferenceList<T>& list) {
    out << quint32(list.size());
    for(int i = 0; i < list.size(); ++i) out << list.at(i);
    return out;
}


/**
* Make sure QDataStream >> is not callable, there is no scenario where it would be useful with a
* ReferenceList.
*/
template<class T>
QDataStream & operator>>(QDataStream &out, const ReferenceList<T>& list) = delete;

#endif
