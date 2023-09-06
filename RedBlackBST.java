import java.security.Key;

public class RedBlackBST<K extends Comparable<K>, V> {
    private static final boolean RED = true;
    private static final boolean BLACK = false;

    private class Node {
        K key;
        V val;
        Node left, right;
        int size;
        boolean color; // color of link from parent to this node

        Node(K key, V val, int size, boolean color) {
            this.key = key;
            this.val = val;
            this.size = size;
            this.color = color;
        }
    }

    Node root;

    RedBlackBST() {
        root = null;
    }

    public int size() {
        return size(root);
    }

    private int size(Node h) {
        return h == null ? 0 : h.size;
    }

    // Return true if link from parent to this node is red
    private boolean isRed(Node x) {
        if (x == null)
            return false;
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
        if (h == null)
            return new Node(key, val, 1, RED);

        int cmp = key.compareTo(h.key);
        if (cmp < 0)
            h.left = put(h.left, key, val);
        else if (cmp > 0)
            h.right = put(h.right, key, val);
        else
            h.val = val;

        balance(h);

        h.size = size(h.left) + size(h.right) + 1;
        return h;
    }

    public boolean isEmpty() {
        return root == null;
    }

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
        // 
        if (isRed(h.left))
            h = rotateRight(h);
        
        if (h.right == null)
            return null;
        
        if (!isRed(h.right) && !isRed(h.right.left))  // right child is 2-node
            h = moveRedRight(h);
        
        h.right = deleteMax(h.right);
        return balance(h);
    }

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

    public void print() {
        print(root);
    }

    private void print(Node h) {
        if (h == null)
            return;
        System.out.println("{ key: " + h.key + ", val: " + h.val + ", color: " + h.color + "} ");
        print(h.left);
        print(h.right);
    }

    public static void main(String[] args) {
        RedBlackBST<Integer, Integer> bst = new RedBlackBST<>();
        bst.put(10, 1);
        bst.put(20, 2);
        bst.put(30, 3);
        bst.put(40, 4);
        bst.put(50, 5);
        bst.put(60, 6);
        bst.put(70, 7);
        bst.print();
    }
}