#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP
#include <algorithm>
#include <type_traits>
#include "misc.hpp"

template<typename T, typename = std::enable_if_t<std::is_copy_constructible_v<T> && std::is_default_constructible_v<T>>>
struct AUSTINUTILS list_node {
    public:

    list_node<T>* next = nullptr;
    list_node<T>* prev = nullptr;
    T x;

    list_node() : x(T()) {}
    list_node(T x) : x(x) {}
    list_node(T x, list_node<T>* prev) : prev(prev), x(x) {}
    list_node(T x, list_node<T>* prev, list_node<T>* next) : next(next), prev(prev), x(x) {}

    list_node(std::initializer_list<T> init_list) {
        if (init_list.size() != 0) {
            auto it = init_list.begin();
            x = *it;
            ++it;
            if (it != init_list.end()) {
                next = new list_node<T>(*it, this);
                list_node<T> *head = next;
                ++it;
                while (it != init_list.end()) {
                    head->next = new list_node<T>(*it, head);
                    head = head->next;
                    ++it;
                }
            }

        }
    }

    list_node(const list_node& other) {
        x = other.x;
        prev = nullptr;
        next = nullptr;

        const list_node<T>* src = &other;
        list_node<T>* dest = this;

        while (src->next) {
            dest->next = new list_node<T>(src->next->x, dest, nullptr);
            dest = dest->next;
            src = src->next;
        }
    }

    list_node(list_node&& other) noexcept {
        x = std::move(other.x);
        prev = other.prev;
        next = other.next;
        other.prev = nullptr;
        other.next = nullptr;
    }

    void push_forward(T x) {
        if (next) {
            auto *node = new list_node<T>(x, this, next);
            this->next->prev = node;
            this->next = node;
        } else {
            this->next = new list_node<T>(x, this);
        }

    }

    void push_backward(T x) {
        if (prev) {
            auto *node = new list_node<T>(x, prev, this);
            this->prev->next = node;
            this->prev = node;
        } else {
            this->prev = new list_node<T>(x, nullptr, this);
        }
    }

    list_node<T>* erase_and_advance() {
        //erase this node and return the next node
        list_node<T>* nextNode = next; // Store next node before deletion

        delete this;
        return nextNode; // Return the next node safely
    }

    list_node<T>* erase_and_retreat() {
        //erase this node and return the previous node
        list_node<T>* prevNode = prev; // Store previous node before deletion

        delete this;
        return prevNode; // Return the previous node safely
    }

    void erase_forward() {
        if (next) {
            auto* ptr = next->next;
            delete next;
            this->next = ptr;
        }
    }

    void erase_backward() {
        if (prev) {
            auto* ptr = prev->prev;
            delete prev;
            this->prev = ptr;
        }
    }

    void deallocate() {
        list_node<T>* node = this;

        // Move to the head of the list
        while (node->prev) {
            node = node->prev;
        }

        // Delete all nodes from the head
        while (node) {
            list_node<T>* nextNode = node->next;
            delete node;// Destructor will unlink nodes before freeing memory
            node = nextNode;
        }
    }

    template<AustinUtils::Formattable U>
    friend std::ostream& operator <<(std::ostream& os, list_node<U>& self) {
        list_node<U>* head = &self;
        list_node<U>* cur = head;

        while (cur->prev) {
            cur = cur->prev;
        }
        while (cur != head) {
            os << cur->x;
            os << " <- ";
            cur = cur->next;
        }

        os << head->x;
        if (head->next) os << " -> ";
        cur = cur->next;
        while (cur) {
            os << cur->x;
            if (cur->next) os << " -> ";
            cur = cur->next;
        }

        return os;
    }


    list_node<T>* goToEnd() {
        if (!this->next) return this;
        auto* head = this;
        while (head->next) {
            head = head->next;
        }
        return head;
    }

    list_node<T>* goToBeginning() {
        if (!this->prev) return this;
        auto* head = prev;
        while (head->prev) {
            head = head->prev;
        }
        return head;
    }

    template<AustinUtils::Formattable Tp>
    friend std::ostream& operator <<(std::ostream& os, list_node<Tp>*& self) {
        os << *self;
        return os;
    }

    //iterates forward
    class iterator {
    private:
        list_node<T>* node;
    public:
        iterator(list_node<T>* node) : node(node) {

        }

        T& operator *() {
            return node->x;
        }

        T* operator ->() {
            return &node->x;
        }

        iterator& operator++() {
            node = node->next;
            return *this;
        }

        iterator operator++(int) {
            iterator ret = iterator(node);
            node = node->next;
            return ret;
        }

        iterator& operator--() {
            node = node->prev;
            return *this;
        }

        iterator operator--(int) {
            iterator ret = iterator(node);
            node = node->prev;
            return ret;
        }

        bool operator==(const iterator &other) const {
            return node == other.node;
        }

        bool operator !=(const iterator& other) const {
            return node != other.node;
        }
    };

    //iterates forward
    class riterator {
    private:
        list_node<T>* node;
    public:
        explicit riterator(list_node<T>* node) : node(node) {

        }

        T& operator *() {
            return node->x;
        }

        T* operator ->() {
            return &node->x;
        }

        riterator& operator++() {
            node = node->prev;
            return *this;
        }

        riterator operator++(int) {
            riterator ret = riterator(node);
            node = node->prev;
            return ret;
        }

        riterator& operator--() {
            node = node->next;
            return *this;
        }

        riterator operator--(int) {
            riterator ret = riterator(node);
            node = node->next;
            return ret;
        }

        bool operator==(const riterator &other) const {
            return node == other.node;
        }

        bool operator !=(const riterator& other) const {
            return node != other.node;
        }
    };

    iterator begin() {
        return iterator(this);
    }

    iterator end() {
        return iterator(nullptr);
    }

    riterator rbegin() {
        return riterator(this);
    }

    riterator rend() {
        return riterator(nullptr);
    }

    void swap(list_node& other) noexcept {
        if (this == other) return; // Self-swap check

        // Swap next pointers and adjust adjacent nodes
        list_node* p = next;
        next = other.next;
        other.next = p;

        if (next) next.prev = this;
        if (other.next) other.next->prev = &other;

        // Swap prev pointers and adjust adjacent nodes
        p = prev;
        prev = other.prev;
        other.prev = p;

        if (prev) prev->next = this;
        if (other.prev) other.prev->next = &other;
    }

    void swap(list_node* other) {
        if (this == other) return; // Self-swap check

        // Swap next pointers and adjust adjacent nodes
        list_node* p = next;
        next = other->next;
        other->next = p;

        if (next) next->prev = this;
        if (other->next) other->next->prev = other;

        // Swap prev pointers and adjust adjacent nodes
        p = prev;
        prev = other->prev;
        other->prev = p;

        if (prev) prev->next = this;
        if (other->prev) other->prev->next = other;
    }



    ~list_node() {
        // Simply unlink this node, do not delete recursively
        if (prev) prev->next = next;
        if (next) next->prev = prev;
    }


};

namespace std {
    template<typename T>
    void swap(list_node<T>* n1, list_node<T>* n2) {
        n1->swap(n2);
    }

    template<typename T>
    void swap(list_node<T>& n1, list_node<T>& n2) noexcept {
        n1.swap(n2);
    }
}

#endif