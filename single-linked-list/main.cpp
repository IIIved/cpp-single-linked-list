#include <cassert>

#include "single-linked-list.h"

using namespace std;

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
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
        explicit BasicIterator(Node* node) {
            node_ = node;
        }

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
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            node_ = other.node_;
        }

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
            (const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
            
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=
            (const BasicIterator<const Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        // Оператор сравнения итераторов (в роли второго аргумента
        // итератор)
        // Два итератора равны, если они ссылаются на один и тот же
        // элемент списка либо на end()
        [[nodiscard]] bool operator==
            (const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=
            (const BasicIterator<Type>& rhs) const noexcept {
            return node_ != rhs.node_;
        }

        // Оператор прединкремента. После его вызова итератор
        // указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий
        // элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            this->node_ = this->node_->next_node;
            return *this;
        }

        // Оператор постинкремента. После его вызова итератор
        // указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий
        // элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
            auto old_value(*this);
            ++(*this);
            return old_value;
        }

        // Операция разыменования. Возвращает ссылку на текущий
        // элемент
        // Вызов этого оператора у итератора, не указывающего на
        // существующий элемент списка, приводит к неопределённому
        // поведению
        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        // Операция доступа к члену класса. Возвращает указатель на
        // текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на
        // существующий элемент списка, приводит к неопределённому
        // поведению
        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) {
        SingleLinkedList tmp;
        for (auto value : values) {
            tmp.PushBack(value);
        }
        swap(tmp);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        assert(size_ == 0 && head_.next_node == nullptr);

        if (head_.next_node != other.head_.next_node) {
            SingleLinkedList tmp;
            for (auto it = other.begin(); it != other.end();
                 ++it) {
                tmp.PushBack(*it);
            }
            swap(tmp);
        }
    }

    // Обменивает содержимое списков за время O(1)
    void swap(SingleLinkedList& other) noexcept {
        auto tmp_node = head_.next_node;
        auto tmp_size = size_;

        head_.next_node = other.head_.next_node;
        size_ = other.size_;

        other.head_.next_node = tmp_node;
        other.size_ = tmp_size;
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (head_.next_node != rhs.head_.next_node) {
            SingleLinkedList tmp;
            for (auto it = rhs.begin(); it != rhs.end();
                 ++it) {
                tmp.PushBack(*it);
            }
            Clear();
            swap(tmp);
        }
        return *this;
    }

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
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{&head_};
    }

    // Возвращает константный итератор, указывающий на позицию
    // перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования
    // приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator(const_cast<Node*>(&head_)); ;
    }

    // Возвращает константный итератор, указывающий на позицию
    // перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования
    // приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{&head_};
    }

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{head_.next_node};
    }

    // Возвращает итератор, указывающий на позицию, следующую за
    // последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования
    // приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        return Iterator{nullptr};
    }

    // Возвращает константный итератор, ссылающийся на первый
    // элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    // Возвращает константный итератор, указывающий на позицию,
    // следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования
    // приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator{nullptr};
    }

    // Возвращает константный итератор, ссылающийся на первый
    // элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    // Возвращает константный итератор, указывающий на позицию,
    // следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования
    // приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{nullptr};
    }

    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // Вставляет элемент value в начало списка за время O(1)
    void PushFront(const Type& value) {
        Node* new_node;
        try {
            new_node = new Node(value, head_.next_node);
        } catch (const std::bad_alloc&) {
            throw std::bad_alloc();
        }
        head_.next_node = new_node;
        ++size_;
    }

    void PushBack(const Type& value) {
        Node* new_node;
        try {
            new_node = new Node(value, nullptr);
        } catch (const std::bad_alloc&) {
            throw std::bad_alloc();
        }
        
        if (last_node_ != nullptr) {
            last_node_ -> next_node = new_node;
        }
        ++size_;
        last_node_ = new_node;
        if (head_.next_node == nullptr) {
            head_.next_node = last_node_;
        }
    }

    /*
     * Вставляет элемент value после элемента, на который указывает
     * pos.
     * Возвращает итератор на вставленный элемент
     * Если при создании элемента будет выброшено исключение,
     * список останется в прежнем состоянии
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_ != nullptr);

        Node* ptr_new_node = new Node(value, pos.node_->next_node);

        pos.node_->next_node = ptr_new_node;
        if (pos.node_->next_node == nullptr) {
            last_node_ = ptr_new_node;
        }
        ++size_;
        return Iterator{ptr_new_node};
    }

    void PopFront() noexcept {
        assert(!IsEmpty());

        Node* ptr_next_node = head_.next_node -> next_node;
        delete head_.next_node;

        if (ptr_next_node == nullptr) {
            last_node_ = nullptr;
        }
        head_.next_node = ptr_next_node;
        --size_;
    }

    /*
     * Удаляет элемент, следующий за pos.
     * Возвращает итератор на элемент, следующий за удалённым
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_ != nullptr &&
               pos.node_ -> next_node != nullptr);

        Node* ptr_node_after_erase = pos.node_ -> next_node -> next_node;
        delete pos.node_ -> next_node;

        pos.node_ -> next_node = ptr_node_after_erase;
        if (ptr_node_after_erase == nullptr) {
            last_node_ = pos.node_;
        }
        --size_;
        return Iterator{ptr_node_after_erase};
    }

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

    ~SingleLinkedList() {
        Clear();
    }

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


// Эта функция проверяет работу класса SingleLinkedList
void Test() {
    struct DeletionSpy {
        ~DeletionSpy() {
            if (deletion_counter_ptr) {
                ++(*deletion_counter_ptr);
            }
        }
        int* deletion_counter_ptr = nullptr;
    };

    // Проверка PopFront
    {
        SingleLinkedList<int> numbers{3, 14, 15, 92, 6};
        numbers.PopFront();
        assert((numbers == SingleLinkedList<int>{14, 15, 92, 6}));

        SingleLinkedList<DeletionSpy> list;
        list.PushFront(DeletionSpy{});
        int deletion_counter = 0;
        list.begin()->deletion_counter_ptr = &deletion_counter;
        assert(deletion_counter == 0);
        list.PopFront();
        assert(deletion_counter == 1);
    }

    // Доступ к позиции, предшествующей begin
    {
        SingleLinkedList<int> empty_list;
        const auto& const_empty_list = empty_list;
        assert(empty_list.before_begin() == empty_list.cbefore_begin());
        assert(++empty_list.before_begin() == empty_list.begin());
        assert(++empty_list.cbefore_begin() == const_empty_list.begin());

        SingleLinkedList<int> numbers{1, 2, 3, 4};
        const auto& const_numbers = numbers;
        assert(numbers.before_begin() == numbers.cbefore_begin());
        assert(++numbers.before_begin() == numbers.begin());
        assert(++numbers.cbefore_begin() == const_numbers.begin());
    }

    // Вставка элемента после указанной позиции
    {  // Вставка в пустой список
        {
            SingleLinkedList<int> lst;
            const auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);
            assert((lst == SingleLinkedList<int>{123}));
            assert(inserted_item_pos == lst.begin());
            assert(*inserted_item_pos == 123);
        }

        // Вставка в непустой список
        {
            SingleLinkedList<int> lst{1, 2, 3};
            auto inserted_item_pos = lst.InsertAfter(lst.before_begin(), 123);

            assert(inserted_item_pos == lst.begin());
            assert(inserted_item_pos != lst.end());
            assert(*inserted_item_pos == 123);
            assert((lst == SingleLinkedList<int>{123, 1, 2, 3}));

            inserted_item_pos = lst.InsertAfter(lst.begin(), 555);
            assert(++SingleLinkedList<int>::Iterator(lst.begin()) == inserted_item_pos);
            assert(*inserted_item_pos == 555);
            assert((lst == SingleLinkedList<int>{123, 555, 1, 2, 3}));
        };
    }

    // Вспомогательный класс, бросающий исключение после создания N-копии
    struct ThrowOnCopy {
        ThrowOnCopy() = default;
        explicit ThrowOnCopy(int& copy_counter) noexcept
            : countdown_ptr(&copy_counter) {
        }
        ThrowOnCopy(const ThrowOnCopy& other)
            : countdown_ptr(other.countdown_ptr)  //
        {
            if (countdown_ptr) {
                if (*countdown_ptr == 0) {
                    throw std::bad_alloc();
                } else {
                    --(*countdown_ptr);
                }
            }
        }
        // Присваивание элементов этого типа не требуется
        ThrowOnCopy& operator=(const ThrowOnCopy& rhs) = delete;
        // Адрес счётчика обратного отсчёта. Если не равен nullptr, то уменьшается при каждом копировании.
        // Как только обнулится, конструктор копирования выбросит исключение
        int* countdown_ptr = nullptr;
    };

    // Проверка обеспечения строгой гарантии безопасности исключений
    {
        bool exception_was_thrown = false;
        for (int max_copy_counter = 10; max_copy_counter >= 0; --max_copy_counter) {
            SingleLinkedList<ThrowOnCopy> list{ThrowOnCopy{}, ThrowOnCopy{}, ThrowOnCopy{}};
            try {
                int copy_counter = max_copy_counter;
                list.InsertAfter(list.cbegin(), ThrowOnCopy(copy_counter));
                assert(list.GetSize() == 4u);
            } catch (const std::bad_alloc&) {
                exception_was_thrown = true;
                assert(list.GetSize() == 3u);
                break;
            }
        }
        assert(exception_was_thrown);
    }

    // Удаление элементов после указанной позиции
    {
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto& const_lst = lst;
            const auto item_after_erased = lst.EraseAfter(const_lst.cbefore_begin());
            assert((lst == SingleLinkedList<int>{2, 3, 4}));
            assert(item_after_erased == lst.begin());
        }
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto item_after_erased = lst.EraseAfter(lst.cbegin());
            assert((lst == SingleLinkedList<int>{1, 3, 4}));
            assert(item_after_erased == (++lst.begin()));
        }
        {
            SingleLinkedList<int> lst{1, 2, 3, 4};
            const auto item_after_erased = lst.EraseAfter(++(++lst.cbegin()));
            assert((lst == SingleLinkedList<int>{1, 2, 3}));
            assert(item_after_erased == lst.end());
        }
        {
            SingleLinkedList<DeletionSpy> list{DeletionSpy{}, DeletionSpy{}, DeletionSpy{}};
            auto after_begin = ++list.begin();
            int deletion_counter = 0;
            after_begin->deletion_counter_ptr = &deletion_counter;
            assert(deletion_counter == 0u);
            list.EraseAfter(list.cbegin());
            assert(deletion_counter == 1u);
        }
    }
}

int main() {
    Test();
}
