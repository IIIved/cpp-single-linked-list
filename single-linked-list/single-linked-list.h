#pragma once

#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>

// добавьте неоходимые include-директивы сюда

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next);
        Type value;
        Node* next_node = nullptr;
    };

    // Шаблон класса «Базовый Итератор».
    // Определяет поведение итератора на элементы односвязного
    // списка
    // ValueType — совпадает с Type (для Iterator) либо с
    // const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов
        // списка был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на
        // узел списка
        explicit BasicIterator(Node* node);

    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о
        // свойствах этого итератора

        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и
        // многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;

        BasicIterator() = default;

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль
        // копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль
        // конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept;

        // Чтобы компилятор не выдавал предупреждение об отсутствии
        // оператора = при наличии пользовательского конструктора
        // копирования, явно объявим оператор = и попросим
        // компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) =
                                 default;

        // Оператор сравнения итераторов (в роли второго аргумента
        // выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же
        // элемент списка либо на end()
        [[nodiscard]] bool operator==
        (const BasicIterator<const Type>& rhs) const noexcept;

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=
        (const BasicIterator<const Type>& rhs) const noexcept;

        // Оператор сравнения итераторов (в роли второго аргумента
        // итератор)
        // Два итератора равны, если они ссылаются на один и тот же
        // элемент списка либо на end()
        [[nodiscard]] bool operator==
        (const BasicIterator<Type>& rhs) const noexcept;

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=
        (const BasicIterator<Type>& rhs) const noexcept;

        // Оператор прединкремента. После его вызова итератор
        // указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий
        // элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept;

        // Оператор постинкремента. После его вызова итератор
        // указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий
        // элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept;

        // Операция разыменования. Возвращает ссылку на текущий
        // элемент
        // Вызов этого оператора у итератора, не указывающего на
        // существующий элемент списка, приводит к неопределённому
        // поведению
        [[nodiscard]] reference operator*() const noexcept;

        // Операция доступа к члену класса. Возвращает указатель на
        // текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на
        // существующий элемент списка, приводит к неопределённому
        // поведению
        [[nodiscard]] pointer operator->() const noexcept;

    private:
        Node* node_ = nullptr;
    };

public:

    SingleLinkedList() = default;
    SingleLinkedList(std::initializer_list<Type> values);
    SingleLinkedList(const SingleLinkedList& other);

    // Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept;
    
    SingleLinkedList& operator=(const SingleLinkedList& rhs);

    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к
    // элементам списка
    using ConstIterator = BasicIterator<const Type>;

    // Возвращает итератор, указывающий на позицию перед первым
    // элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования
    // приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept;

    // Возвращает константный итератор, указывающий на позицию
    // перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования
    // приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept;

    // Возвращает константный итератор, указывающий на позицию
    // перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования
    // приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept;

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept;

    // Возвращает итератор, указывающий на позицию, следующую за
    // последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования
    // приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept;

    // Возвращает константный итератор, ссылающийся на первый
    // элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept;

    // Возвращает константный итератор, указывающий на позицию,
    // следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования
    // приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept;

    // Возвращает константный итератор, ссылающийся на первый
    // элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept;

    // Возвращает константный итератор, указывающий на позицию,
    // следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования
    // приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept;

    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept;

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept;

    // Вставляет элемент value в начало списка за время O(1)
    void PushFront(const Type& value);

    void PushBack(const Type& value);

    /*
     * Вставляет элемент value после элемента, на который указывает
     * pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение,
     * список останется в прежнем состоянии
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value);

    void PopFront() noexcept;

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept;
    // Очищает список за время O(N)
    void Clear() noexcept {
        Node* next_node;
        while (head_.next_node != nullptr) {
            next_node = head_.next_node -> next_node;
            delete head_.next_node;
            head_.next_node = next_node;
        }
        size_ = 0;
    }

    ~SingleLinkedList();

private:
    // Фиктивный узел, используется для вставки
    // "перед первым элементом"
    Node head_;
    size_t size_ = 0;
    Node* last_node_ = nullptr;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs,
          SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) { return true; }
    auto rhs_it = rhs.begin();
    for (auto it = lhs.begin(); it != lhs.end(); ++it) {
        if (*it != *rhs_it) { return true; }
        ++rhs_it;
    }
    return false;
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs,
               const SingleLinkedList<Type>& rhs) {
    if (std::lexicographical_compare(lhs.begin(), lhs.end(),
                                     rhs.begin(), rhs.end())) {
        return true;
    }
    return false;
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
    if (std::lexicographical_compare(rhs.begin(), rhs.end(),
                                     lhs.begin(), lhs.end())) {
        return false;
    }
    return true;
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs,
               const SingleLinkedList<Type>& rhs) {
    if (std::lexicographical_compare(rhs.begin(), rhs.end(),
                                     lhs.begin(), lhs.end())) {
        return true;
    }
    return false;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs,
                const SingleLinkedList<Type>& rhs) {
    if (std::lexicographical_compare(lhs.begin(), lhs.end(),
                                     rhs.begin(), rhs.end())) {
        return false;
    }
    return true;
}

