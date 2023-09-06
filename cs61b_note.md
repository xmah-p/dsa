[TOC]

# Misc


## 命令行

`javac file` 将 `file` 编译为字节码文件 `file.class`。

`java file` 运行 `file.class`。如果 `file` 文件在某个 `package` 中，则需要在 `file` 的上级目录中通过 canonical name 运行，例如 `java capers.Main`。

## Git

`git checkout commitID`：将 `HEAD` 切换到某个提交（进入 HEAD-detached 状态），并将仓库恢复成此提交时的状态。之后的修改不会对仓库产生影响。此时可以执行 `git checkout -b new-branch-name` 创建新的分支（在 HEAD-detached 后修改的工作区内容和暂存区状态不会改变）

`git clone <remote_repo_url>`：克隆整个远程仓库，克隆后将作为 `origin` 添加到本地仓库的 `remote` 中

`git remote add <remote_name> <remote_repo_url>`：在本地仓库添加一个新的远程仓库

`git remote -v`：查看远程仓库

`git push <remote_name> <branch_name>`：将本地修改提交到远程仓库的 `branch_name` 分支

`git fetch <remote_name>`：将远程仓库的修改拉取到本地仓库的远程跟踪分支上

`git merge <branch_name>`：将 `branch_name` 分支合并到当前分支

`git pull <remote_name> <branch_name>`：`git fetch` + `git merge`，将远程仓库 `branch_name` 分支拉取合并到当前分支，当**没有需要合并的内容**时，发生 `Fast-forward`，此时 Git 将 `HEAD` 简单地指向 `branch_name` 分支的最新提交

`git restore <file_name>`：撤销工作区中 `file_name` 的修改，恢复到暂存区的状态

`git restore --staged <file_name>`：将暂存区中的 `file_name` 恢复成上一次提交的状态，如果 `file_name` 是上次提交以来的新文件，则等同于 `git rm --cached <file_name>`

`git rm --cached <file_name>`：从暂存区中移除 `file_name`，停止追踪它，不删除工作区中的文件，下次提交时，此文件会在仓库中被删除（如果之前有此文件）。可以用 `git restore --staged <file_name>` 撤销

`git rm <file_name>`：从暂存区中移除 `file_name`，并删除工作区中的文件



## 测试

```java
import org.junit.Test;

public class testArrayMap {
    @Test
    public void testArrayMap0() {
        ArrayMap<String, Integer> am = new ArrayMap<String, Integer>();
        am.put("hello", 5);
        int expected = 5;
        assertEquals(expected, (int) am.get("hello"));    // 注意类型转换
    }
}
```

## 异常

```java
throw new IllegalArgumentException("can't add null");
```

```java
try {
    // do something
} catch (IllegalArgumentException e) {
    // do something
} catch (Exception e) { // catch all exceptions
    // do something
} finally {
    // do something
}
```

有的异常必须被捕获（checked exceptions），例如 `IOException`。如果代码不处理这些异常，编译器会报错。

<!--  generate dot code to illustrate the hierarchy of Throwable -->
```dot
digraph {
    Throwable -> Exception [dir=back]
    Throwable -> Error [dir=back]
    Error -> "AssertionError" [dir=back]
    Error -> "Other Errors..." [dir=back]
    Exception -> RuntimeException [dir=back]
    Exception -> IOException [dir=back]
    Exception -> "Other Excetions..." [dir=back]
    RuntimeException -> NullPointerException [dir=back]
    RuntimeException -> IllegalArgumentException [dir=back]
    RuntimeException -> "Other RuntimeExceptions..." [dir=back]
}
```

除了 `Error` 和 `RuntimeException`，其他异常都是 checked exceptions。

```java
// 如果没有在方法内 catch 异常，就必须 throws 声明
public static void gulgate() throws IOException {
... throw new IOException("hi"); ...
}
```



## 变参

```java
// 注意，这个方法是静态的，因此它不知道自己所在类的类型参数
// 我们需要为它指定类型参数 T
public static <T> ArraySet<T> of(T... args) {
    ArraySet<T> res = new ArraySet<T>();
    for (T x : args) {
        res.add(x);
    }
    return res;
}
```


# 继承

## Access Control

未指定 package 时，当前类默认属于 default package，其他 package 的类无法访问 default package 中的类。

- Private ：和 C++ 相同
- Package Private：未指定访问控制符时的默认项，只有同一个 `package` 中的类可以访问，**但不能被子类访问**
- Protected：只有同一个 package 中的类以及自己的子类可以访问
- Public：和 C++ 相同

对于**接口**来说，默认的访问权限是 `public`，而不是 `package-private` 的。

和 C++ 一样，访问权限检查是基于**对象的静态类型**的。

```java
package universe;
public interface BlackHole {
    void add(Object x); // this method is public, not package-private!
}

package universe;
public class CreationUtils {
    public static BlackHole hirsute() {
         return new HasHair();
    }
}

package universe;
class HasHair implements BlackHole {    // package-private
    Object[] items;
    public void add(Object o) { ... }
    public Object get(int k) { ... }
}

import static CreationUtils.hirsute;
class Client {
   void demoAccess() {
      BlackHole b = hirsute();
      b.add("horse");    // ok
      b.get(0);          // error! get() is package-private, b can't access it even though b is a HasHair
      HasHair hb = (HasHair) b;    // error! Client has no idea what HasHair is
      // 判断方法：完全依照静态类型
   }
}
```

例如，`iterator()` 方法可以返回一个 `private` 的 `KeyIterator` 动态类型的对象，而 `KeyIterator` 类型是 `HashMap` 的内部类。

```java
public class ArrayMap<K, V> {
    // ...

    // 返回类型也可以是 KeyIterator
    public Iterator<K> iterator() {
        return new KeyIterator();
    }

    private class KeyIterator implements Iterator<K> {
        // ...
    }
}
```


## `implements`


```java
public interface List61B<Item> {
    public void addFirst(Item x);
    public void add Last(Item y);
    public Item getFirst();
    public Item getLast();
    public Item removeLast();
    public Item get(int i);
    public void insert(Item x, int position);
    public int size();

    // default 关键字允许在接口中提供方法定义
    default public void print() {
        for (int i = 0; i < size(); i++) {
            System.out.print(get(i) + " ");
        }
        System.out.println();
    }
}

public class SLList<Item> implements List61B<Item> {
    @Override
    public void addFirst(Item x) {
        ...
    }

    // 重写接口中的默认方法
    @Override
    public void print() {
        ...
    }
}
```

`interface` 的特性：
- 所有方法都必须 `public`
- 所有变量都必须 `public static final`
- 不可实例化
- 方法默认都是 `abstract` 的，除非指定 `default`
- 一个类可以实现多个接口



## `extends`

```java
public class RotatingSLList<Item> extends SLList<Item> {
    RotatingSLList() {
        // no-arg constructor super() will be called automatically by java if we don't call it explicitly
        super();
    }

    RotatingSLList(Item x) {
        super(x);    // must call explicitly, because java will only call super(), not super(x)
    }

    public void rotateRight();
}
```

java 中的所有类（`class`）都（隐式地）`extends` 了 `Object` 类，但接口（`interface`）不 `extends` `Object` 类。

## 多态


注意，重载函数并不适用多态关系：

```java
// Animal is an interface, Dog implements Animal
public static void func(Animal a) {
    System.out.println("Animal");
    a.say();    
}

public static void func(Dog d) {
    System.out.println("Dog");
    d.say();
}

public static void main(String[] args) {
    Animal a = new Dog();
    Dog d = new Dog();
    func(a);     // call func(Animal a), but inside func, it calls d.say()
    func(d);     // call func(Dog d)
}
// 这和 C++ 是一样的
```

多态的一些注意事项：

```java
class SLList<Item> {
    public void addLast(Item x) { ... }
    public void removeLast() { ... }
}

class VengefulSLList<Item> extends SLList<Item> {
    SLList<Item> deletedItems;

    @Override
    public Item removeLast() { ... }


    public void printLostItems() { ... }
}

public static void main(String[] args) {
    VengefulSLList<Integer> vsl = new VengefulSLList<Integer>(1);
    SLList<Integer> sl = vsl;    // ok, 基类引用指向派生类对象

    sl.addLast(50);    // ok，调用基类方法
    sl.removeLast();    // ok，调用派生类方法

    sl.printLostItems();    // 编译 error！基类没有这个方法
    VengefulSLList<Integer> vsl2 = sl;    // 编译 error！派生类引用不能指向基类对象
}
```

## Casting

```java
public static Animal maxAnimal(Animal a1, Animal a2);

public static void main(String[] args) {
    Animal a = new Dog();
    Dog d = new Dog();
    Dog maxd = maxAnimal(a, d);    // 编译 error！函数返回类型为 Animal
    Dog maxd = (Dog) maxAnimal(a, d);    // ok，强制类型转换

    MyDog md = new Dog();   // 编译 error！派生类引用不能指向基类对象
    MyDog md = (MyDog) new Dog();    // 编译ok，运行时异常！

    //...

    // 利用 instanceof 可以实现安全的向下转型
    if (d instanceof MyDog) {
        MyDog md = (MyDog) d;
    }
}
```

## `Comparable` 和 `Comparator`

java 中，有的对象之间可比较，有的不能。我们可以为一个类实现 `compateTo` 方法，使其对象可比较。然而不同类对 `compareTo` 方法的实现不一定相同，这为编写统一的操作函数（比如说，`max(Object a, Object b)` 函数）带来了困难。与其为 `Dog` 类实现 `maxDog`，再为 `Cat` 类实现 `maxCat`，不如令类的比较规范和统一。java 提供了 `Comparable` 接口，它要求派生类实现 `compareTo` 方法。

```java
public interface Comparable<T> {
    public int compareTo(T o);
}

public class Dog implements Comparable<Dog> {
    @Override
    // positive if >, negative if <, 0 if ==
    public int compareTo(Dog uddaDog) {
        return this.size - uddaDog.size;
    }
}
```

Natural order：`compareTo` 方法定义下的顺序。

假如我们想要将一个 `Dog[]` 按**不同于自然序**的顺序排序，我们可以利用 `Comparator`：

```java
// Comparator interface
public interface Comparator<T> {
    // positive if >, negative if <, 0 if ==
    public int compare(T o1, T o2);
}
```

```java
import java.util.Comparator;

public class Dog implements Comparable<Dog> {
    // ...
    public int compareTo(Dog uddaDog);

    // 私有 静态 嵌套类
    private static class NameComparator implements Comparator<Dog> {
        @Override
        public int compare(Dog a, Dog b) {
            return a.name.compareTo(b.name);
        }
    } 

    // getter
    public static Comparator<Dog> getNameComparator {
        return new NameComparator();
    }
}
```

## `abstract class`

`abstract class` 的特性：
- 方法可以是 `public` 或 `private`
- 变量不必是 `public static final`，可以是任意类型的
- 不可实例化
- 方法默认是 concrete 的，除非指定 `abstract`
- 一个类只能 `extends` 一个 `abstract class`

# Generics and autoboxing

## Autoboxing

| Primitive type | Wrapper class |
| :------------: | :-----------: |
|      byte      |     Byte      |
|      short     |     Short     |
|       int      |    Integer    |
|      long      |     Long      |
|      float     |     Float     |
|     double     |    Double     |
|     boolean    |    Boolean    |
|      char      |    Character  |

```java
public class BasicArrayList {
    public static void main(String[] args) {
      ArrayList<Integer> L = new ArrayList<Integer>();
      // L.add(new Integer(5));
      L.add(5);         // autoboxing: implicitly convert between primitives and wrapper classes

      /* Use the Integer.valueOf method to convert to int */
      // int first = L.get(0).valueOf();
      int first = L.get(0);    // autounboxing
    }
}
```

- 数组没有 autoboxing 和 autounboxing。`int[]` 和 `Integer[]` 之间不能简单替换
- 依赖 autoboxing 和 autounboxing 可能导致性能下降和存储空间浪费

## Widening

期待 `double` 的地方可以直接用 `int` 替代，但期待 `int` 的地方，`double` 不能直接替代，需要手动类型转换。

## Immutability

被声明为 `final` 的变量是 `const` 的。引用被声明为 `final`，不代表其指向的对象是 `const` 的。

```java
// deque 是 const 引用，但其指向的对象不是 const 的
public final ArrayDeque<String>() deque = new ArrayDeque<String>();
```
（不能像 C++ 一样直接声明一个不可变的引用对象）

## Generics

```java
// 必须在返回值前指定类型参数
public static <K,V> V get(Map61B<K,V> map, K key) {
    if map.containsKey(key) {
        return map.get(key);
    }
    return null;
}

// 和 C++ 不同，Java 对泛型的编译检查更严格
public static <K extends Comparable<K>, V> K maxKey(Map61B<K, V> map) {
    List<K> keylist = map.keys();
    K largest = map.get(0);
    for (K k: keylist) {
        if (k.compareTo(largest)) {
            largest = k;
        }
    }
    return largest;
}
```

`extends` 在泛型中的语义是“限制”，在继承与多态中的语义则是“扩展”。这里，虽然 `K` 实际上是 “`implements`” 了 `Comparable` 接口，但我们仍然使用 `extends`。



# Java 库

## `java.lang`

### `Object`

- `clone()`：一般应实现为深拷贝，即使 `x.clone() != x && x.clone().getClass() == x.getClass()`。`Object` 的默认实现只是简单地拷贝了对象的字段，即浅拷贝。
- `equals(Object o)`
- `getClass()`：返回运行时类型
- `hashCode()`
- `toString()`

`System.out.println(x)` 会隐式地调用 `x.toString()`。

`Object` 对 `toString()` 的默认实现会打印对象的内存地址。

`equals()` 和 `==` 有不同的语义。Java 中，`==` **检查两个对象是否指向同一个内存地址**（对 primitives 来说，检查值是否相等）。

`Object` 对 `equals()` 的默认实现就是 `==`。我们可以重写 `equals()` 以改变语义。

```java
public boolean equals(Object other) {
    if (this == other) return true;
    if (other == null) return false;
    if (other.getClass() != this.getClass()) return false;

    ArraySet<T> o = (ArraySet<T>) other;  // cast
    if (o.size() != this.size()) return false;
    for (T item : this) {
        if (!o.contains(item)) return false;
    }
    return true;
}
```

`equals()`：
- 必须拥有**自反性、对称性、传递性**
- 参数类型必须是 `Object`
- 若 `x.equals(y)`，那么只要 `x` 和 `y` 不变，`x.equals(y)` 的结果也不变
- `x.equals(null)` 必须返回 `false`


### `String`

和 C++ 不同的是，`"abc"` 这种字符串字面量也属于 `String`，`"abc".length()` 是合法的。

继承自 `Object` 的类都提供了 `toString` 方法。

一般地，向 `String` 类的函数传递 `null` 参数会抛出 `NullPointerException`。



- `Substring(int beginIndex, int endIndex=string.length())`
- `charAt(int index)`
- `indexOf(int ch, int fromIndex=0)`
- `indexOf(String str, int fromIndex=0)`：它们返回第一次出现的索引，如果没有找到，返回 `-1`。
- `concat(String str)`：效率比 `+` 高，但 `+` 可以实现字符串和其他类型的拼接
- `replace(char oldChar, char newChar)`
- `trim()`：去除首尾空格后的字符串


- （静态）`valueOf(Arg a)`：参数类型可以是 `boolean`、`char`、`int`、`long`、`float`、`double` 等等
- （静态）`format(String format, Object... args)`：格式化字符串

- `compareTo(String anotherString)`
- `compareToIgnoreCase(String str)`
- `startsWith(String prefix, int toffset=0)`：检查是否以 `prefix` 开头
- `endsWith(String suffix)`：检查是否以 `suffix` 结尾
- `equalsIgnoreCase(String anotherString)`
- `isEmpty()`
- `matches(String reges)`

一个天真的 `toString` 实现：

```java
public String toString() {
    String retStr = "{";
    for (int i = 0; i < size; i++) 
        retStr += items[i] + ", ";
    retStr += "}";
    return retStr;
}
```

由于 `String` 是不可变的，每次 `+=` 都会创建一个新的 `String` 对象，因此这种实现的时间复杂度是 `O(n^2)`。正确的实现应该使用 `StringBuilder`：

```java
public String toString() {
    StringBuilder retSB = new StringBuilder("{");
    for (int i = 0; i < size - 1; i++) 
        retSB.append(items[i].toString());
        retSB.append(", ");
    retSB.append(items[size - 1].toString());
    retSB.append("}");
    return retSB.toString();
}
```

利用 `String.join`：

```java
public String toString() {
    List<String> ls = new ArrayList<>();
    for (T x : this) 
        ls.add(x.toString());
    
    StringBuilder retSB = new StringBuilder("{");
    retSB.append(String.join(", ", ls));
    return retSB.append("}").toString();
}
```

## `java.util`

### 接口 `List`

- `add(int index=List.size(), E e)`：`true` 表示添加成功，对不允许重复的情况，`false` 表示参数已经在 `List` 中，添加失败。
- `addAll(int index=List.size(), Collection<? extends E> c)`
- `clear()`：清除所有
- `remove(int index)`：返回被删除的元素
- `remove(Object o)`：删除首个 `o`，返回是否删除成功
- `removeAll(Collection<?> c)`：删除所有 `c` 中的元素
- `retainAll(Collection<?> c)`：只保留所有 `c` 中的元素
- `set(int index, E element)`：返回被替换的元素
- `contains(Object o)`
- `containsAll(Collection<?> c)`
- `get(int index)`
- `indexOf(Object o)`
- `lastIndexOf(Object o)`
- `toArray()`：返回 `Object[]`
- `toArray(T[] a)`：如果装得下，结果装在 `a` 中并被返回，否则返回新的数组。返回值类型是 `T[]`，而不是 `Object[]。

- `iterator()`：返回 `Iterator<E>`
- `listIterator(int index=0)`：返回 `ListIterator<E>`


## `Iterable`

增强 `for` 循环

```java
Set<String> s = new HashSet<>();
// ...
for (String item: s) {
    // ...
}

// 等价于

Iterator<String> seer = s.iterator();
while (seer.hasNext()) {
    String item = seer.next();    // String item = seer++;
    // ...
}
```

`Iterable` 是一个接口：

```java
public interface Iterable<T> {
    Iterator<T> iterator();
}
```

`List` 继承自 `Collection`，而 `Collection` 继承自 `Iterable`。

`Iterator` 接口要求了 `hasNext` 和 `next` 方法，

```java
public interface Iterator<T> {
    boolean hasNext();
    T next();
}
```

当 `hasNext()` 返回 `false` 时，`next()` 的行为是未定义的（通常会抛出 `NoSuchElementException`）


## 文件

```java
File f = new File("dummy.txt");  // 这并不会创建 dummy.txt 文件本身

f.createNewFile();  // 创建文件
f.exists();         // 文件是否存在

File d = new File("dummy");  // 目录
d.exists();                  // 目录是否存在
d.mkdir();                   // 创建目录
```

### Serialization

```java
import java.io.Serializable;

public class Model implements Serializable {
    // Serializable 是一个标记接口 没有方法
    // ...
}
```

以下代码将 `m` 转换成字节流并写入 `model.save`：

```java
Model m = new Model();
File outFile = new File("model.save");
try {
    ObjectOutputStream out = new ObjectOutputStream(new FileOutputStream(outFile));
    out.writeObject(m);
    out.close();
} catch (IOException e) {
    // ...
}
```

以下代码重新从 `model.save` 中读取字节流并转换成 `Model` 对象：

```java
Model m;
File inFile = new File("model.save");
try {
    ObjectInputStream inp = new ObjectInputStream(new FileInputStream(inFile));
    m = (Model) inp.readObject();
    inp.close();
} catch (IOException | ClassNotFoundException e) {
    // ...
    m = null;
}
```




# Union-Find / Disjoint Set

问题：给定多组整数对作为输入，每个整数对表示两个对象。如果两个对象在同一个整数对中，则它们是连通等价的（即有自反性、对称性、传递性）。设计一个算法，从输入中过滤掉所有无意义的整数对。

我们将每个整数称为一个触点（site），整数对关系称为连接（connection），连通的等价类称为分量（component）。

## QuickFind

```java
public class QuickFind implements UF {

    private int[] id; // id[site] = component id
    private int count; // amount of components

    // Init sites with 0 to N-1
    public QuickFind(int N) {
        count = N;
        id = new int[N];
        for (int i = 0; i != N; i++)
            id[i] = i;
    }

    @Override
    public void union(int p, int q) {
        // replace components containing p with component containing q
        // component id == one random site member
        int pid = find(p);
        int qid = find(q);
        if (pid == qid)
            return;
        // 此处为算法瓶颈：必须遍历整个数组以找出所有与 p 相连的触点
        // 同时注意到 id[p] 实际上是个毫无意义的值，考虑通过改变数据结构来优化掉遍历
        for (int i = 0; i != id.length; i++) {
            if (id[i] == pid)
                id[i] = qid;
        }
        count--;
    }

    @Override
    public int find(int p) {
        return id[p];
    }

    // amount of components
    @Override
    public int count() {
        return count;
    }

    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        int N = s.nextInt();
        UF uf = new QuickFind(N);
        while (s.hasNext()) {
            int p = s.nextInt();
            int q = s.nextInt();
            if (uf.connected(p, q))
                continue;
            uf.union(p, q);
            System.out.println(p + " " + q);
        }
        System.out.println(uf.count() + " components");
        s.close();
    }
}
```

在quick-find 算法中，每次 `find()` 调用只需要访问数组一次，而归并两个分量的 `union()` 操作访问数组的次数在 $(N+3)$ 到 $(2N+1)$ 之间（`union()` 调用两次 `find()`，检查 $N$ 个数组的值，改变其中 $1$ 到 $N-1$ 个值）。算法的最好情况是 $(N+3)(N-1) ~ N^2$ 次访问，最坏情况是 $(2N+1)(N-1) ~ 2N^2$ 次访问。


## QuickUnion

优化：将 `id` 数组看作一棵树，`id[site]` 的值为 `site` 的父结点，规定根结点的父结点为自身（即 `id[root] == root`）。

```java
// find roots of p and q, link one to the other 
@Override
public void union(int p, int q) {
    int proot = find(p);
    int qroot = find(q);

    if (proot == qroot) return;
    id[proot] = qroot;    // 归并两棵树非常简单
    count--;
}

// now, component id == root of site
@Override
public int find(int p) {
    while (id[p] != p)
        p = id[p];
    return p;
}
```


定义**树的大小**为树中的结点数，某结点的**深度**是它到根结点的链接数。树的**高度**是它的所有节点中的最大深度。则 `find()` 操作访问数组的次数为 $1 + 2\operatorname{depth}(site)$，`connected()` 访问次数为 `find()` 的两倍，`union()` 访问次数为 `find()` 的两倍加 $0$ 或 $1$。

改良后，`find()` 需要访问 $1$ 到 $2N+1$ 次（当然，这通常会被编译优化为常数次）数组。最坏情况发生在整个网络只有一个分量，且所有结点连成一个线性表时。


## Weighted QuickUnion

我们会发现，算法的好坏取决于**树的高度**，只要使树的高度尽可能矮，算法的性能就会更好。因此优化的点在于：**避免大树被连接到小树的根结点上**。

通过保证大树不会被连接到小树的根结点，我们可以保证**树的高度**不会超过 $\lg N$。这样，`find()` 操作的访问次数就不会超过 $1 + 2\lg N$，`connected()` 和 `union()` 操作的访问次数就不会超过 $3 + 4\lg N$。

> 网络中，大小为 $k$ 的树的高度不超过 $\lg k$。因为将小树（大小记为 $i$）连接到大树（大小记为 $j$）的根结点后，原大树结点深度不变，原小树结点深度加 $1$，新树（大小为 $k=i+j$）深度不超过 $\lg i + 1 = 2\lg i \le \lg (i+j) = \lg k$。我们的优化本质上基于“每次让最少的结点深度增加”。

```java
public class WeightedQuickUnion implements UF {

    private int[] id;  // parent of i
    private int[] sz;  // size of component of root of i
    private int count; // amount of components

    public WeightedQuickUnion(int N) {
        count = N;
        id = new int[N];
        sz = new int[N];
        for (int i = 0; i != N; ++i) {
            id[i] = i;
            sz[i] = 1;
        }
    }

    @Override
    public void union(int p, int q) {
        int pid = find(p);
        int qid = find(q);
        if (pid == qid) return;
        if (sz[pid] < sz[qid]) {
            id[pid] = qid;    // small tree's root links to large tree's root

            // After this, depth of every site in the small tree increased by 1
            // depth of every site in the large tree remained the same
            // So essentially, our optimization is to make sure that
            // the LEAST amount of sites have their depth increased
            
            // Why only update sz[qid]?
            // We only care about size of a root, 
            // because we only access the size array by a ROOT index
            // Thus we don't care what any sz[NonRoot] is -- we'll never use it
            sz[qid] += sz[pid];    
        } else {
            id[qid] = pid;
            sz[pid] += sz[qid];
        }
        count--;
    }

    @Override
    public int find(int p) {
        if (p != id[p])
            p = id[p];
        return p;
    }

    @Override
    public int count() {
        return count;
    }
}
```

更多的优化：**路径压缩**。

在 `find()` 中添加一个循环，将路径上遇到的所有结点都链接到根结点，这相当于**备忘录**，既然我们**只关心根结点是否相同**，那么其他叶结点的信息便可以丢弃了，让每个“没用”的叶结点充当到根结点的快速移动通道，从而达到优化的目的。


以上的三种算法就是**并查集**（Disjoint Set）的实现。



# Binary Search Tree

## Basics

考虑一个有序单链表，它需要线性时间才能完成 `contains()` 和 `add()` 操作：

```dot
digraph {
    rankdir=LR;
    Sent [shape=box, label="Sentinel"];
    Sent -> A;
    A -> B;
    B -> C;
    C -> D;
    D -> E;
    E -> F;
    F -> G;
}
```

为了提高效率，我们改变链表的结构，于是搜索时间减半了！

```dot
digraph {
    rankdir=LR;
    A -> B [dir=back];
    B -> C [dir=back];
    C -> D [dir=back];
    D -> E;
    E -> F;
    F -> G;
    Sent [shape=box, label="Sentinel"];
    Sent -> D;
}
```

递归地这样做：

```dot
digraph {
    D -> B
    B -> A
    B -> C
    D -> F
    F -> E
    F -> G
}
```

**树**：由结点（Node）和连接（Edge）组成的数据结构。树的**任意两个结点之间都有且仅有一条路径**。

我们可以为树指定**根结点**，根结点没有父结点。

二叉树：每个结点最多有两个子结点。

BST Property：对每个结点 $X$，其左子树中的所有结点都小于 $X$，其右子树中的所有结点都大于 $X$。

BST 中的序关系必须有完全性、传递性和反对称性，BST **不允许重复的 Key**。

```java
// Pseuodo java code with naked recursion
// In this sample, a BST is equivalent to its root node
public class BST<K extends Comparable<K>> {

    private K key;
    private BST left;
    private BST right;
    private BST parent;    // just for successor()

    public BST(K key, BST left, BST Right) {
        this.key = key;
        this.left = left;
        this.right = right;
    }

    public BST(K key) {
        this.key = key;
    }

    static BST find(BST T, K sk) {
        if (T == null)
            return null;
        if (sk.equals(T.key))
            return T;
        else if (sk.compareTo(T.key) < 0)
            return find(T.left, sk);
        else
            return find(T.right, sk);
    }

    // return a new BST with ik inserted
    static BST insert(BST T, K ik) {
        // base case of recursion
        if (T == null) return new BST(ik);
        
        // DO NOT WRITE:
        // if (n.left == null) n.left = new Node(key, val, 1);
        // else if (n.right == null) n.right = new Node(key, val, 1);
        // It just make code ugly, and what if n == null? You'll find the code above doesn't do your any good
        // TRUST THE RECURSION

        // Always set left and right pointers, even if nothing is actually changing
        if (ik.compareTo(T.key) < 0) T.left = insert(T, ik);
        else if (ik.compareTo(T.key) > 0) T.right = insert(T.right, ik);
        return T;
    }

    // return a new BST with dk deleted
    static <K extends Comparable<K>> BST delete(BST T, K dk) {
        if (T == null) return null;

        if (dk.compareTo(T.key) < 0) T.left = delete(T.left, dk);
        else if (dk.compareTo(T.key) > 0) T.right = delete(T.right, dk);
        else {
            // T is the node to be deleted
            if (T.left == null) return T.right;
            if (T.right == null) return T.left;

            // T has two children
            // Find the smallest node in T's right subtree (the successor)
            // and replace T with it
            BST suc = T.right;
            while (suc.left != null) suc = suc.left;
            T.key = suc.key;

            // delete suc (optimizable because of repeated pathing for suc)
            T.right = delete(T.right, suc.key);  
        }
    }

    static <K extends Comparable<K>> BST successor(BST T) {
        // if T has a right child, then return the min of its right subtree
        if (T.right != null) {
            BST suc = T.right;
            while (suc.left != null) suc = suc.left;
            return suc;
        } 
        // otherwise return the first ancestor greater than T
        // or null if T is the largest node
        else {
            BST suc = T.parent;
            while (suc != null && suc.right == T) {
                T = suc;
                suc = suc.parent;
            }
            return suc;
        }
    }
}
```

如何删除结点？

- 待删结点没有子结点：将它的父连接删除即可
- 待删结点有一个子结点：将它的父连接指向它的子结点
- 待删结点有两个子结点：用它的后继结点（右子树中的最小结点）或前驱结点（左子树中的最大结点）替换它，然后删除后继结点或前驱结点（Hibbard deletion）

为什么？回忆顺序单链表，根结点就是单链表中的**中间值**，因此，新的根结点一定是**左子树中的最大值**或**右子树中的最小值**。

容易发现前两种情况只是第三种情况的特例。


### 复杂度分析

**树的大小**为树中的结点数。

某结点的**深度（depth）**是它到根结点的链接数。

树的**平均深度（average depth）**是所有结点深度的平均值。

树的**高度（height）**是它的所有节点中的最大深度。


对于“bushy”的 BST 来说，其高度 $H=\Theta(\lg N)$，但对于“spindly”的 BST，其高度 $H=\Theta(N)$，这是 BST 的最坏情况：退化成顺序单链表。

对于一般的 BST，如果按顺序插入结点（1 -> 2 -> 3 -> ... -> 7），那么我们会得到一棵“spindly”的 BST（插入的每个结点都比父结点大 / 小，因此都会成为右 / 左子结点）；如果按由中间向两端的顺序插入结点（4 -> 2 -> 1 -> 3 -> 6 -> 5 -> 7），那么我们会得到一棵“bushy”的 BST。相同大小的 BST，后者的平均深度要小得多。

> 随机地插入结点，我们得到的 BST 的平均深度 $\sim 2 \ln N$，高度 $\sim 4.311 \ln N$
> 再加上随机地删除结点的操作，我们的 BST 的平均深度和高度是 $\Theta(\sqrt N)$ 的
> 然而我们的 BST 仍然可能是“spindly”的，因为实践中的插入和删除经常是有顺序的


## B Trees

一般 BST 的麻烦在于，我们**总是在叶结点下插入结点**，这导致树的高度增加，平衡结构也被破坏。

$\text{CRAZY IDEA}$：那就不要在叶结点下插入结点了！相反，我们把新的 key 塞进已有的叶结点中。

```dot
digraph {
    4 -> 2 -> 1
    2 -> 3
    4 -> 6 -> 5
    6 -> "7    8    9    10"
}
```

于是，树的高度不再增加，平衡结构得到了保持。然而我们注意到，叶结点会变得 overstuffed，退化成一个顺序单链表。

为每个结点设置最多可容纳的 key 数目 $L$，当 key 数超过 $L$ 时，**我们就把结点分裂成两个结点，然后把中间的键提升到父结点中**。

例如对于 $L=3$，我们的 BST 会变成这样：

```dot
digraph {
    4 -> 2 -> 1
    2 -> 3
    4 -> "6    8" -> 5
    "6    8" -> 7
    "6    8" -> "9    10"
}
```
通过 $\text{overstuff}$ 和 $\text{splitting}$，我们让 BST 始终保持了完美的平衡。这样的树就是 **B 树**。特殊地，当 $L=2$ 时，B 树又叫 **2-3 树**（表明每个结点可以有 $2$ 或 $3$ 个子结点）；当 $L=3$ 时，B 树就是 **2-3-4 树**（每个结点可以有 2、3 或 4 个子结点）。

B 树的 $\text{invariants}$：
- **所有叶结点的深度相同**
- **每一个有 $k$ 个 $\text{key}$ 的非叶结点都一定有 $k+1$ 个子结点**

这两条 $\text{invariants}$ 保证了 B 树永远是 bushy 的，其高度 $H=O(\lg N)$，插入和查找的复杂度都是 $O(\lg N)$。





## BST Map

```java
public class BSTMap<K extends Comparable<K>, V> implements Map61B<K, V> {

    Node root;

    public BSTMap() {

    }

    @Override
    public void clear() {
        root = null;
    }

    @Override
    public boolean containsKey(K key) {
        return containsKey(root, key);
    }

    private boolean containsKey(Node n, K key) {
        if (n == null) return false;
        int cmp = key.compareTo(n.key);
        if (cmp < 0) return containsKey(n.left, key);
        else if (cmp > 0) return containsKey(n.right, key);
        return true;
    }

    @Override
    public V get(K key) {
        return get(root, key);
    }

    private V get(Node n, K key) {
        if (n == null) return null;
        int cmp = key.compareTo(n.key);
        if (cmp < 0) return get(n.left, key);
        if (cmp > 0) return get(n.right, key);
        return n.val;
    }

    @Override
    public int size() {
        return size(root);
    }

    private int size(Node n) {
        return n == null ? 0 : n.size;
    }

    @Override
    public void put(K key, V value) {
        root = put(root, key, value);
    }

    private Node put(Node n, K key, V value) {
        if (n == null) return new Node(key, value, 1);
        int cmp = key.compareTo(n.key);
        if (cmp < 0) n.left = put(n.left, key, value);
        else if (cmp > 0) n.right = put(n.right, key, value);
        n.size = size(n.left) + size(n.right) + 1;  // 注意更新 size 的逻辑
        return n;
    }

    public void printInOrder() {
        printInOrder(root);
        System.out.println();
    }

    private void printInOrder(Node n) {
        if (n == null) return;
        System.out.print("{ " + n.key + ", " + n.val + " }");
        printInOrder(n.left);
        printInOrder(n.right);
    }

    @Override
    public Set<K> keySet() {
        Set<K> set = new HashSet<>();
        keySet(root, set);
        return set;
    }

    private void keySet(Node n, Set<K> set) {
        if (n == null) return;
        set.add(n.key);
        keySet(n.left, set);
        keySet(n.right, set);
    }

    @Override
    public V remove(K key) {
        V ret = get(key);
        root = remove(root, key);
        return ret;
    }

    @Override
    public V remove(K key, V value) {
        V ret = get(key);
        if (ret != value) return null;
        root = remove(root, key);
        return ret;
    }

    private Node remove(Node n, K key) {
        if (n == null) return null;
        int cmp = key.compareTo(n.key);
        if (cmp < 0) n.left = remove(n.left, key);
        else if (cmp > 0) n.right = remove(n.right, key);
        else {
            if (n.left == null) return n.right;
            if (n.right == null) return n.left;

            Node t = n.right;
            while (t.left != null) {
                t = t.left;
            }
            n.key = t.key;
            n.val = t.val;
            n.right = remove(n.right, t.key);
        }
        n.size = size(n.left) + size(n.right) + 1;
        return n;
    }

    @Override
    public Iterator<K> iterator() {
        throw new UnsupportedOperationException();
    }

    private class Node {
        K key;
        V val;
        int size;

        Node left, right;

        Node(K k, V v, int sz) {
            key = k;
            val = v;
            size = sz;
            left = right = null;
        }
    }
}
```

# Left-Leaning Red Black Tree


## Rotating Trees

一共有 `Catalan(N)` 个大小为 `N` 的 BST。

`rotateLeft(G): Let x be the right child of G. Make G the new left child of x.`

`rotateRight(G): Let x be the left child of G. Make G the new right child of x.`

```dot
digraph {
    G -> C -> A -> B;
    G -> P -> K -> J
    P -> R;
    K -> L;
}
```

经过 `rotateLeft(G)` 后：

```dot
digraph {
    P -> G -> C -> A -> B;
    G -> K -> J;
    P -> R;
    K -> L;
}
```

**根结点 `G` 和它的右子结点 `P` 交换位置（把 `P` 提上去），`P` 的左子树 `K` 成为 `G` 的右子树。**

可以看成是暂时地“merge”了 `P` 和 `G`，然后将 `G` 下放成了左子结点。

`rotateRight(P)` 可以看成是 `rotateLeft(G)` 的逆操作：**根结点 `P` 和它的左子结点 `G` 交换位置，`G` 的右子树 `K` 成为 `P` 的左子树。**

```dot
digraph {
    1 -> 3 -> 2;
}
```

经过 `rotateRight(3)` 和 `rotateLeft(1)` 后，我们得到了一棵平衡的 BST：

```dot
digraph {
    2 -> 1;
    2 -> 3;
}
```


## 将 2-3 树表示为 BST

显然，只有 **2-结点**（即只有两个子结点的结点）的 2-3 树本身就是一个 BST。

```dot
digraph {
    m -> "d    f" -> b;
    "d    f" -> e;
    "d    f" -> g;
    m -> o -> n;
    o -> p;
}
```

对于 **3-结点**，我们创建一个**红色的胶水连接**，并把**较小的 key 变成左子树**。

```dot
digraph {
    m -> f
    f -> d [color=red];
    d -> b;
    d -> e;
    f -> g;
    m -> o -> n;
    o -> p;
}
```

这样的树就是一个 **Left-Leaning Red Black Binary Search Tree（LLRB）**。

**LLRB 和 2-3 树之间是一一对应的。**
  
如果一棵 2-3 树的高度是 $H$，那么它对应的 LLRB 的高度最多为 $H+(H+1)=2H+1$（黑连接加红连接）。

- **一个结点不可能被两个红连接接触**，否则它在 2-3 树里就会对应一个 4-结点，这是不可能的
- **每个叶结点到根结点之间的黑连接数相同，称为黑色平衡性**。因为 2-3 树的所有叶结点深度相同

利用以上两个特性，可以判断 LLRB 是否有效。

## 插入

**插入时，用红连接**。因为 2-3 树在插入时总是将新的 key 塞进已有的叶结点。

**插入导致了右红连接，将新结点的父结点左旋**。这个操作并不会体现在对应的 2-3 树上。

```dot
digraph {
    B -> A;
    B -> E;
    E -> S [color=red];
}
```

经过 `rotateLeft(E)` 后：

```dot
digraph {
    B -> A;
    B -> S;
    S -> E [color=red];
}
```

就像 2-3 树一样，插入时可能创造出“临时 4-结点”：

```dot
digraph {
    B -> A;
    B -> S;
    S -> E [color=red];
    S -> T [color=red];
}
```

然而，连续两次向左边插入会创造出错误的临时 4-结点：

```dot
digraph {
    B -> A;
    B -> Z;
    Z -> S -> E [color=red];
}
```

我们通过 `rotateRight(Z)` 将其修正：

```dot
digraph {
    B -> A;
    B -> S;
    S -> E [color=red];
    S -> Z [color=red];
}
```

在 2-3 树中，我们通过分裂 overstuffed 的结点，将中间的键提升到父结点中解决临时 4-结点。例如：

```dot
digraph {
    G -> "A  B  C";
    G -> X;
}
```

将会变成

```dot
digraph {
    "B  G" -> A;
    "B  G" -> X;
}   
```

对于 LLRB，这个操作对应于**颜色翻转**：

```dot
digraph {
    G -> B;
    G -> X;
    B -> A [color=red];
    B -> C [color=red];
}
```

我们用 `flip(B)` 将**所有接触 B 的连接的颜色翻转**：

```dot
digraph {
    G -> B [color=red];
    G -> X;
    B -> A;
    B -> C;
}
```

小结：
- **插入时，用红连接**
- **右红连接，左旋父结点**
- **连续两个左红连接，右旋祖父结点**
- **两个子结点都是红连接，翻转结点的颜色**

**旋转和颜色翻转都是局部操作，不会影响整棵树的黑色平衡性**。

![](images/红黑树3-结点插入.png)

## 代码实现

```java
public class RedBlackBST<K extends Comparable<K>, V> {
    private static final boolean RED = true;
    private static final boolean BLACK = false;

    private class Node {
        K key;
        V val;
        Node left, right;
        int size;
        boolean color;  // color of link from parent to this node

        Node(K key, V val, int size, boolean color) {
            this.key = key;
            this.val = val;
            this.size = size;
            this.color = color;
        }
    }

    Node root;

    public int size() {
        return size(root);
    }

    private int size(Node h) {
        return h == null ? 0 : h.size;
    }

    // Return true if link from parent to this node is red
    private boolean isRed(Node x) {
        if (x == null) return false;
        return x.color;
    }

    private Node rotateLeft(Node h) {
        // rotate node
        Node n = h.right;
        h.right = n.left;
        n.left = h;
        // update size
        h.size = size(h.left) + size(h.right) + 1;
        n.size = size(n.left) + size(n.right) + 1;
        // swap color
        boolean tmp = h.color;
        h.color = n.color;
        n.color = tmp;
        return n;
    }

    private Node rotateRight(Node h) {
        // rotate node
        Node n = h.left;
        h.left = n.right;
        n.right = h;
        // update size
        h.size = size(h.left) + size(h.right) + 1;
        n.size = size(n.left) + size(n.right) + 1;
        // swap color
        boolean tmp = h.color;
        h.color = n.color;
        n.color = tmp;
        return n;
    }

    private void flip(Node h) {
        h.color = !h.color;
        h.left.color = !h.left.color;
        h.right.color = !h.right.color;
    }

    public void put(K key, V val) {
        root = put(root, key, val);
        root.color = BLACK;
    }

    private Node put(Node h, K key, V val) {
        if (h == null) return new Node(key, val, 1, RED);

        int cmp = key.compareTo(h.key);
        if (cmp < 0) h.left = put(h.left, key, val);
        else if (cmp > 0) h.right = put(h.right, key, val);
        else h.val = val;

        if (isRed(h.right) && !isRed(h.left)) h = rotateLeft(h);
        if (isRed(h.left) && isRed(h.left.left)) h = rotateRight(h);
        if (isRed(h.left) && isRed(h.right)) flip(h);

        h.size = size(h.left) + size(h.right) + 1;
        return h;
    }
}
```


## 删除

### 2-3-4 树的插入算法

- 如果**根结点是 4-结点**，将它分裂成三个 2-结点
- 在向下查找的过程中，**如果当前结点是 4-结点，且父结点是 2-结点或 3-结点**，将当前结点分裂为两个 2-结点，将中间的键提升到父结点中
- 到达树底后，我们只会遇到 2-结点或 3-结点，因此可以直接插入新的键

用红黑树实现此算法：
- 将 4-结点表示成**由 3 个 2-结点组成的平衡子树**，根结点和两个子结点都用红连接相连
- 在向下查找的过程中分裂所有 4-结点并进行颜色翻转
- 在向上的过程中和插入操作相同，用旋转将 4-结点配平

代码实现惊人地简单：只需要移动一行代码！

```java
private Node put(Node h, K key, V val) {
    if (h == null) return new Node(key, val, 1, RED);
   
    if (isRed(h.left) && isRed(h.right)) flip(h);

    int cmp = key.compareTo(h.key);
    if (cmp < 0) h.left = put(h.left, key, val);
    else if (cmp > 0) h.right = put(h.right, key, val);
    else h.val = val;

    if (isRed(h.right) && !isRed(h.left)) rotateLeft(h);
    if (isRed(h.left) && isRed(h.left.left)) rotateRight(h);

    h.size = size(h.left) + size(h.right) + 1;
    return h;
}
```

### 删除 2-3 树的最小键

从树底的 3-结点删除键是显然的，但 2-结点不是。为了保证我们不会遇到从 2-结点删除一个键这样糟糕的事情，我们沿着左连接向下进行变换，确保当前结点不是 2-结点。


- 如果**根结点是 2-结点且它的两个子结点都是 2-结点**，将它们合并成一个 4-结点
- 如果**根结点和它的左子结点是 2-结点，且它的右子结点是 3-结点**，将根结点的键移至左子结点，将右子结点的最小键提升到根结点中

在沿着左连接向下的过程中：

- 如果当前结点的左子结点不是 2-结点，完成
- **如果当前结点的左子结点是 2-结点，且它的兄弟结点不是 2-结点**，将它的兄弟结点中的一个键移至当前结点中
- **如果当前结点的左子结点是 2-结点，且它的兄弟结点也是 2-结点**，将它们和它们的父结点的最小键合并成一个 4-结点

最后我们会得到一个含有最小键的 3-结点或 4-结点，我们直接进行删除，然后再**回头向上分解所有临时的 4-结点**。

![](images/删除2-3树最小键.png)

### 删除红黑树的最小键

首先，如果根结点的左子结点是 2-结点，将根结点设为红色。

当前结点是 2-结点且它的两个子结点都是 2-结点：

```dot
digraph {
    b [color=red]
    b -> a;
    b -> c;
}
```

对 `b` 做一次颜色翻转：

```dot
digraph {
    b -> a [color=red];
    b -> c [color=red];
}
```

当前结点和它的左子结点是 2-结点，且它的右子结点是 3-结点：

```dot
digraph {
    b [color=red];
    b -> a;
    b -> d;
    d -> c [color=red];
    d -> e;
}
```

颜色翻转 `b`，右旋 `d`，左旋 `b`，颜色翻转 `b`：

```dot
digraph {
    c [color=red];
    c -> b;
    c -> d;
    b -> a [color=red];
    d -> e;
}
```

Shamelessly stolen from [here](https://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html)


```java
public void deleteMin() {
    if (!isRed(root.left) && !isRed(root.right))
        root.color = RED;
    root = deleteMin(root);
    if (!isEmpty())
        root.color = BLACK;
}

private Node deleteMin(Node h) {
    if (h.left == null)
        return null;    // delete 

    if (!isRed(h.left) && !isRed(h.left.left))  // left child is 2-node
        h = moveRedLeft(h);

    h.left = deleteMin(h.left);
    return balance(h);
}

private Node balance(Node h) {
    if (isRed(h.right) && !isRed(h.left))
        h = rotateLeft(h);    
    if (isRed(h.left) && isRed(h.left.left))
        h = rotateRight(h);
    if (isRed(h.left) && isRed(h.right))
        flip(h);

    h.size = size(h.left) + size(h.right) + 1;
    return h;
}

private Node moveRedLeft(Node h) {
    // Assume h.color == RED &&
    // h.left.color == h.left.left.color == BLACK, 
    // i.e., h is a 3-node and h.left is a 2-node
    flip(h);
    if (isRed(h.right.left)) {
        h.right = rotateRight(h.right);
        h = rotateLeft(h);
        flip(h);
    }
    return h;
}
```

方法 `moveRedLeft(h)` 确保 `h.left` 成为 3-结点或 4-结点。

### 删除任意结点

删除最大值的算法：

```java
private Node moveRedRight(Node h) {
    // Assume h.color == RED &&
    // h.right.color == h.right.left.color == BLACK, 
    // i.e., h is a 3-node and h.right is a 2-node
    flip(h);
    if (isRed(h.left.left)) {  // h.left is a 3-node
        h = rotateRight(h);
        flip(h);
    }
    return h;
}

public void deleteMax() {
    if (!isRed(root.left) && !isRed(root.right))
        root.color = RED;
    
    root = deleteMax(root);
    if (!isEmpty())
        root.color = BLACK;
}

private Node deleteMax(Node h) {
    // When deleteMin, it won't happen that isRed(h.right) == true
    // because it is a left-leaning red-black tree
    // Without this line, balance() will fail 
    // because we create double right red links with moveRedRight(h)
    if (isRed(h.left))
        h = rotateRight(h);
    
    if (h.right == null)
        return null;
    
    if (!isRed(h.right) && !isRed(h.right.left))  // right child is 2-node
        h = moveRedRight(h);
    
    h.right = deleteMax(h.right);
    return balance(h);
}
```

将这两种算法结合起来：

```java
public void delete(K key) {
    if (!contains(key)) return;

    // if both children of root are black, set root to red
    if (!isRed(root.left) && !isRed(root.right))
        root.color = RED;

    root = delete(root, key);
    if (!isEmpty()) root.color = BLACK;
}

private Node delete(Node h, K key) {
    // Assume key is in the tree

    if (key.compareTo(h.key) < 0)  {
        if (!isRed(h.left) && !isRed(h.left.left))  // left child is 2-node
            h = moveRedLeft(h);  // make h.left 3-node
        h.left = delete(h.left, key);
    }
    else {
        if (isRed(h.left))
            h = rotateRight(h);

        if (key.compareTo(h.key) == 0 && (h.right == null))
            return null;  // if h.right == null, then h is a red leaf node, safe to delete

        if (!isRed(h.right) && !isRed(h.right.left))  // right child is 2-node
            h = moveRedRight(h);

        if (key.compareTo(h.key) == 0) {
            // replace h with its successor, and delete the successor
            Node x = h.right;
            while (x.left != null)
                x = x.left;
            h.key = x.key;
            h.val = x.val;
            h.right = deleteMin(h.right);
        }
        else h.right = delete(h.right, key);
    }
    return balance(h);
}
```

# Red Black Tree

红黑性质：
- 结点是红色或黑色的
- 根结点是黑色的
- 叶结点（`NIL`）是黑色的
- 如果一个结点是红色的，那么它的两个子结点都是黑色的
- 对每个结点，从该结点到其所有后代叶结点的简单路径上，均包含相同数目的黑色结点

对红黑树 `T`，其哨兵 `T.nil` 代表了此红黑树的所有叶结点（不同于上一章的定义，此处的叶结点指空结点）。`T.nil` 的 `color` 字段为 `BLACK`，其他字段的取值任意。根结点的父结点也是 `T.nil`。

定义从某个结点 `x` 出发（不含此结点）到一个**叶结点**的任意简单路径上的黑色结点数目为结点 `x` 的**黑高（black-height）**，记作 `bh(x)`。

```py
def LEFT-ROTATE(T, x):
    y = x.right
    # turn y's left subtree into x's right subtree
    x.right = y.left
    if y.left != T.nil:
        y.left.p = x
    # link x's parent and y
    y.p = x.p
    if x.p == T.nil:
        T.root = y
    elif x == x.p.left:
        x.p.left = y
    else:
        x.p.right = y
    # put x on y's left
    y.left = x
    x.p = y
```

```py
def RB-INSERT(T, z):
    y = T.nil
    x = T.root
    while x != T.nil:
        y = x
        if z.key < x.key:
            x = x.left
        else:
            x = x.right
    # link y and z
    z.p = y
    if y == T.nil:
        T.root = z
    elif z.key < y.key:
        y.left = z
    else:
        y.right = z
    z.left = T.nil
    z.right = T.nil
    z.color = RED
    RB-INSERT-FIXUP(T, z)

def RB-INSERT-FIXUP(T, z):
    while z.p.color == RED:
        # z.p is left child
        if z.p == z.p.p.left:
            y = z.p.p.right

            # case 1: 
            #           z.p.p(BLACK)
            #           /       \
            #      z.p(RED)     y(RED)
            #      /        \
            # T.nil/z(RED)  z(RED)/T.nil
            if y.color == RED:
                # flip(z.p.p)
                z.p.color = BLACK
                y.color = BLACK
                z.p.p.color = RED
                z = z.p.p

            # case 2
            #           z.p.p(BLACK)
            #           /       \
            #      z.p(RED)     y(BLACK)
            #      /        \
            #   T.nil      z(RED)
            elif z == z.p.right:
                z = z.p
                LEFT-ROTATE(T, z)

            # case 3
            #           z.p.p(BLACK)
            #           /       \
            #      z.p(RED)     y(BLACK)
            #      /        \
            #   z(RED)      T.nil
            z.p.color = BLACK
            z.p.p.color = RED
            RIGHT-ROTATE(T, z.p.p)
        else:
            y = z.p.p.left
            if y.color == RED:
                z.p.color = BLACK
                y.color = BLACK
                z.p.p.color = RED
                z = z.p.p
            elif z == z.p.left:
                z = z.p
                RIGHT-ROTATE(T, z)
            z.p.color = BLACK
            z.p.p.color = RED
            LEFT-ROTATE(T, z.p.p)
    T.root.color = BLACK
```

在调用 `RB-INSERT-FIXUP` 时，可能被破坏的红黑性质只有**性质 2**（根结点为黑色）和**性质 4**（红结点的子结点为黑色）。

在 `while` 循环内，以下不变式始终成立：
- `z` 是红结点
- 若 `z.p` 是根结点，则 `z.p` 是黑结点
- 至多只有一条红黑性质被破坏：要么性质 2，要么性质 4。如果性质 2 被破坏，其原因为 `z` 是根结点且是红结点。如果性质 4 被破坏，其原因为 `z.p` 和 `z` 都是红结点


```py
# replace u with v
def RB-TRANSPLANT(T, u, v):
    # link v to u.p
    if u.p == T.nil:
        T.root = v
    elif u == u.p.left:
        u.p.left = v
    else:
        u.p.right = v
    # link u.p to v
    v.p = u.p
```

```py
def RB-DELETE(T, z):
    y = z
    y-original-color = y.color
    if z.left == T.nil:
        x = z.right
        RB-TRANSPLANT(T, z, z.right)
    elif z.right == T.nil:
        x = z.left
        RB-TRANSPLANT(T, z, z.left)
    else:
        y = TREE-MINIMUM(z.right)  # y is z's successor
        y-original-color = y.color
        x = y.right
        if y.p == z:
            x.p = y
        else:
            RB-TRANSPLANT(T, y, y.right) # delete y
            # y takes over z's right child
            y.right = z.right
            y.right.p = y
        # replace z with y
        # y takes over z's left child
        RB-TRANSPLANT(T, z, y)
        y.left = z.left
        y.left.p = y
        y.color = z.color
    if y-original-color == BLACK:
        RB-DELETE-FIXUP(T, x) 
        # x is the node that takes over y's original position: nil or y.right
```

- 始终维持 `y` 为从树中删除的结点或移至树内的结点
- 如果 `y-original-color` 是黑色的，那么红黑树的红黑性质可能被破坏，此时调用 `RB-DELETE-FIXUP` 来恢复性质
    - 如果 `y` 是原来的根结点，而 `y` 的红色孩子成为了新的根结点，那么性质 2 被破坏
    - 如果 `x` 和 `x.p` 是红色的，则性质 4 被破坏
    - 在树中删除 `y` 导致先前包含 `y` 的任何简单路径上黑结点数少 1，因此 `y` 的任何祖先不满足性质 5。我们通过将现在接管 `y` 位置的结点 `x` 染上**第二重黑色**来解决此问题（这一重黑色不反映在 `x.color`）。从而问题转换为结点 `x` 违反性质 1。
    - 性质 5 将在整个过程中得到保护。


```py
def RB-DELETE-FIXUP(T, x):
    while x != T.root and x.color == BLACK:
        if x == x.p.left:
            w = x.p.right    # w is x's sibling
            # w can't be nil because x is double black

            # case 1
            #          x.p(BLACK)
            #          /       \
            #     x(D_BLACK)     w(RED)
            #                   /    \
            #             w.left    w.right        
            if w.color == RED:
                # This forces w.left and w.right to be black
                # We escape to case 2, 3 or 4
                # Rotate x.p to the left
                w.color = BLACK
                x.p.color = RED
                LEFT-ROTATE(T, x.p)
                w = x.p.right
            
            # case 2
            #         x.p(RED/BLACK)
            #         /       \
            #    x(D_BLACK)     w(BLACK)
            #                  /       \
            #            w.left(BLACK)  w.right(BLACK)
            if w.left.color == BLACK and w.right.color == BLACK:
                # Remove one black from both x and w
                # Add one black to x.p
                w.color = RED
                x = x.p
            
            # case 3
            #         x.p(RED/BLACK)
            #         /       \
            #    x(D_BLACK)     w(BLACK)
            #                  /       \
            #            w.left(RED)  w.right(BLACK)
            elif w.right.color == BLACK:
                # Rotate w to the right
                # Escape to case 4
                w.color = RED
                RIGHT-ROTATE(T, w)
                w = x.p.right

            # case 4
            #         x.p(RED/BLACK)
            #         /       \
            #    x(D_BLACK)     w(BLACK)
            #                   /        \
            #       w.left(RED/BLACK)  w.right(RED)
            # Rotate x.p to the left
            w.color = x.p.color
            x.p.color = BLACK
            w.right.color = BLACK
            LEFT-ROTATE(T, x.p)
            x = T.root

        else:
            w = x.p.left
            if w.color == RED:
                w.color = BLACK
                x.p.color = RED
                RIGHT-ROTATE(T, x.p)
                w = x.p.left
            if w.right.color == BLACK and w.left.color == BLACK:
                w.color = RED
                x = x.p
            elif w.left.color == BLACK:
                w.right.color = BLACK
                w.color = RED
                LEFT-ROTATE(T, w)
                w = x.p.left
            w.color = x.p.color
            x.p.color = BLACK
            w.left.color = BLACK
            RIGHT-ROTATE(T, x.p)
            x = T.root
    x.color = BLACK
```

只有情况 2 会导致 `while` 循环重复执行。