#pragma once
#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <memory>
#include <cstddef>
#include <iterator>

namespace sjtu {

template <typename T>
class list {
  protected:
    struct node_base {
        node_base *prev;
        node_base *next;
        node_base() : prev(this), next(this) {}
    };

    struct node : public node_base {
        T data;
        node(const T &val) : node_base(), data(val) {}
    };

    node_base *head;
    size_t sz;

    void init() {
        head = new node_base();
        sz = 0;
    }

    void destroy() {
        clear();
        delete head;
    }

  public:
    class iterator;
    class const_iterator;

    list() {
        init();
    }

    list(const list &other) {
        init();
        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
    }

    list &operator=(const list &other) {
        if (this == &other) return *this;
        clear();
        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
        return *this;
    }

    ~list() {
        destroy();
    }

    T &front() noexcept {
        return static_cast<node*>(head->next)->data;
    }

    T &back() noexcept {
        return static_cast<node*>(head->prev)->data;
    }

    const T &front() const noexcept {
        return static_cast<node*>(head->next)->data;
    }

    const T &back() const noexcept {
        return static_cast<node*>(head->prev)->data;
    }

    iterator begin() noexcept {
        return iterator(head->next, this);
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(head->next, this);
    }

    iterator end() noexcept {
        return iterator(head, this);
    }

    const_iterator cend() const noexcept {
        return const_iterator(head, this);
    }

    bool empty() const noexcept {
        return sz == 0;
    }

    size_t size() const noexcept {
        return sz;
    }

    void clear() noexcept {
        node_base *curr = head->next;
        while (curr != head) {
            node_base *next = curr->next;
            delete static_cast<node*>(curr);
            curr = next;
        }
        head->next = head;
        head->prev = head;
        sz = 0;
    }

    iterator insert(iterator pos, const T &value) {
        node_base *curr = pos.ptr;
        node *new_node = new node(value);
        new_node->prev = curr->prev;
        new_node->next = curr;
        curr->prev->next = new_node;
        curr->prev = new_node;
        sz++;
        return iterator(new_node, this);
    }

    iterator erase(iterator pos) noexcept {
        node_base *curr = pos.ptr;
        node_base *next_node = curr->next;
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
        delete static_cast<node*>(curr);
        sz--;
        return iterator(next_node, this);
    }

    void push_front(const T &value) {
        insert(begin(), value);
    }

    void push_back(const T &value) {
        insert(end(), value);
    }

    void pop_front() noexcept {
        erase(begin());
    }

    void pop_back() noexcept {
        erase(iterator(head->prev, this));
    }

    class iterator {
      public:
        node_base *ptr;
        const list *lst;

        iterator() : ptr(nullptr), lst(nullptr) {}
        iterator(node_base *p, const list *l) : ptr(p), lst(l) {}

        iterator &operator++() {
            ptr = ptr->next;
            return *this;
        }

        iterator &operator--() {
            ptr = ptr->prev;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }

        T &operator*() const noexcept {
            return static_cast<node*>(ptr)->data;
        }

        T *operator->() const noexcept {
            return &(static_cast<node*>(ptr)->data);
        }

        friend bool operator==(const iterator &a, const iterator &b) {
            return a.ptr == b.ptr;
        }

        friend bool operator!=(const iterator &a, const iterator &b) {
            return a.ptr != b.ptr;
        }
    };

    class const_iterator {
      public:
        node_base *ptr;
        const list *lst;

        const_iterator() : ptr(nullptr), lst(nullptr) {}
        const_iterator(node_base *p, const list *l) : ptr(p), lst(l) {}
        const_iterator(const iterator &other) : ptr(other.ptr), lst(other.lst) {}

        const_iterator &operator++() {
            ptr = ptr->next;
            return *this;
        }

        const_iterator &operator--() {
            ptr = ptr->prev;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }

        const_iterator operator--(int) {
            const_iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }

        const T &operator*() const noexcept {
            return static_cast<node*>(ptr)->data;
        }

        const T *operator->() const noexcept {
            return &(static_cast<node*>(ptr)->data);
        }

        friend bool operator==(const const_iterator &a, const const_iterator &b) {
            return a.ptr == b.ptr;
        }

        friend bool operator!=(const const_iterator &a, const const_iterator &b) {
            return a.ptr != b.ptr;
        }
    };
};

} // namespace sjtu

#endif // SJTU_LIST_HPP