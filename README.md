# PseudoScript

PseudoScript is an interpreted language, aimed at being simple to understand and write, while maintiaining a high level of functionality.

---

Below is the desired way of writing PseudoScript code, and development will be oriented around this example:

```txt
myInt = 5;
myFloat = 3.1415
myBool = true;
myList = [123, "456", true, false];

myFunction(arg1, arg2) {
    mySum = arg1 + arg2;
    return mySum / 2;
}

MyClass {
    __init__(name, age) {
        this.name = name;
        this.age = age;
    }

    getUsername() {
        return this.name.lower() + string(this.age)
    }
}

myObject = MyClass("Joe Bloggs", 50);
print(myObject.getUsername())
```

Note: Language design is subject to change
