#include <iostream>
#include <memory>

class Toy{
  std::string _name;
public:
    Toy(std::string name) : _name(name){}
    Toy() : Toy("SomeToy"){}
    std::string GetName(){
        return _name;
    }


};

class PointerCount{
    Toy* pToy;
    int pCount;
public:
    PointerCount(Toy* inpToy = nullptr) : pToy(inpToy), pCount(0){}
    void addPointer(){
        pCount++;
    }
    void releasePointer(){
        if(!--pCount){
            delete pToy;
            delete this;
        }
    }
    int GetCount(){
        return pCount;
    }
};

class shared_ptr_toy{
    Toy *obj;
    PointerCount* pCount;
public:
    shared_ptr_toy(Toy* inObject = nullptr) : obj(inObject){
        pCount = new PointerCount(obj);
        pCount->addPointer();
    }
    shared_ptr_toy(const shared_ptr_toy& other){
         obj = other.obj;
         pCount = other.pCount;
         if(pCount) pCount->addPointer();
     }
     shared_ptr_toy& operator=(const shared_ptr_toy& other){
         if(this == &other) return *this;
         if(this->pCount) this->pCount->releasePointer();
         obj = other.obj;
         pCount = other.pCount;
         if(pCount) pCount->addPointer();
         return *this;
     }
     ~shared_ptr_toy(){
        if(pCount) pCount->releasePointer();
    }
    Toy& operator*(){
        return *obj;
    }
    Toy* operator->(){
        return obj;
    }
    int GetPointerCount(){
        return pCount->GetCount();
    }
};

shared_ptr_toy make_shared_toy(Toy& toy){
    return shared_ptr_toy(new Toy(toy));
}

shared_ptr_toy make_shared_toy(std::string toyName){
    return shared_ptr_toy(new Toy(toyName));
}

int main() {
    Toy newToy("Druzok");
    Toy* toy1 = new Toy("Ball");
    shared_ptr_toy c = make_shared_toy("Snow");
    std::cout << " c pointer count: " << c.GetPointerCount() << std::endl;
    shared_ptr_toy d = make_shared_toy(newToy);
    std::cout << " d pointer count: " << d.GetPointerCount() << std::endl;
    {
        shared_ptr_toy a(toy1);
        std::cout << a->GetName() << std::endl;
        std::cout << " a pointer count: " << a.GetPointerCount() << std::endl;
        Toy toy = *c;
        std::cout << toy.GetName() << std::endl;
        {

            shared_ptr_toy b = a;
            std::cout << b->GetName() << std::endl;
            std::cout << " b(= a) pointer count: " << b.GetPointerCount() << std::endl;
            b = c;
            std::cout << " b(= c) pointer count: " << b.GetPointerCount() << std::endl;
            std::cout << " a pointer count: " << a.GetPointerCount() << std::endl;

        }

    }
    std::cout << d->GetName() << std::endl;
    std::cout << c->GetName() << std::endl;
    return 0;
}
