
//-------------------array based------------------

// queue

template <typename T, unsigned SIZE>
class myAQueue
{
public:
    explicit myAQueue() : qSize(SIZE), qFront(0), qBack(0)
    {
        q = new T[qSize];
    }
    ~myAQueue()
    {
        if (q) delete[] q;
    }
    bool deque(T& x);
    void enque(T x);
    bool isEmpty(){ return (qFront <= qBack);}
private:
    T* q;
    int qSize;
    int qFront, qBack;
};

template <typename T>
bool myAQueue<T>::deque(T& x)
{
    if(qFront <= qBack){
        return false;
    }

/*
 *if  SIZE = power of 2 then
 ++qBack;
qBack &=  0x0000....FFF...; <- SIZE of array
same for qFront
 * */

    x = q[(qBack++ % qSize)];
    return true;
}

template <typename T>
void myAQueue<T>::enque(T x)
{
    q[(qFront++ % qSize)] = x;
}

// stack

template <typename T>
class myAStack
{
public:
    explicit myAStack(int num) : sSize(num), counter(-1)
    {
        s = new T[sSize];
    }
    ~myAStack()
    {
        if (s) delete[] s;
    }
    void pop(T&);
    void push(T);
    bool isEmpty(){ return(counter <= -1); }
private:
    T* s;
    int sSize;
    int counter;
};

template <typename T>
void myAStack::pop(T& x)
{
    if(counter <= -1){
        cout << "The stack is empty!\n";
        return;
    }
    x = s[(counter-- % sSize)];
}

template <typename T>
void myAStack::push(T x)
{
    s[(++counter % sSize)] = x;
}

//----------------link-list based------------

// queue

template <typename T>
class MyQueue
{
  struct Node
    {
        Node *next;
        T val;
    };
public:
    MyQueue() : head(new Node), tail(head)
    {
    }
    myQueue(const MyQueue&) = delete;
    void operator=(const MyQueue&) = delete;
    myQueue(MyQueue&& rhs);
    myQueue& operator=(MyQueue&& rhs);
    ~MyQueue()
    {
        while (top){
        Node* deleteMe = top;
        top = top->next;
        delete deleteMe;
        }
    }
    T deque();
    void enque(const T arg);
    bool isEmpty(){return (head == tail);}
private:
    Node *head, *tail;

};


template <typename T>
inline void MyQueue<T>::enque(const T arg)
{
    tail->val = arg;
    Node* new_tail = new Node;
    tail->next = new_tail;
    tail = new_tail;
}

template <typename T>
inline T MyQueue<T>::deque()
{
    if (head == tail) throw;

    Node* old_head = head;
    head = old_head->next;
    T res = old_head->val;
    delete old_head;

    return res;
}

// stack

template <typename T>
class MyStack
{
  struct Node
    {
        Node *next;
        T val;
    };
public:
    MyStack() : top(nullptr){}
    ~MyStack();
    T pop();
    void push(const T& arg);
    bool isEmpty() { return (top) ? false : true; }
private:
    Node *top;

};

template <typename T>
inline MyStack<T>::~MyStack()
{
    while (top){
        Node* deleteMe = top;
        top = top->next;
        delete deleteMe;
    }
}

template <typename T>
inline void MyStack<T>::push(const T&arg)
{
    Node* node = new Node();
    node->val = arg;

    node->next = top;
    top = node;
}

template <typename T>
inline T MyStack<T>::pop()
{
    if (!top) {
        cerr << "Stack is empty!\n";
        throw;
    }
    T result = top->val;

    Node* deleteMe = top;
    top = top->next;
    delete deleteMe;

    return result;
}
