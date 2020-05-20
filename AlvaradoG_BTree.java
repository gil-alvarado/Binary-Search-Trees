//GILBERT ALVARADO
//COMPILER: intelliJ
//JRE: 8.0
import java.util.Random;
import java.util.Scanner;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                          SIMULATOR
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
public class AlvaradoG_BTree {

    public static void main(String[] args) {

        System.out.println("BST");
        
        Random rand = new Random();//CONDITION 1
        int []numbers = new int[20];
        System.out.print("REQ1\nORIGINAL SEQUENCE:\n[ ");
        for(int i = 0; i < numbers.length; i++) {
            numbers[i] = rand.nextInt(99) + 1;
            System.out.print(numbers[i] + " ");
        }
        System.out.print("]\n");

        System.out.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        BST tree = new BST(numbers);//condition 2
        tree.printInOrder();
        System.out.println("------------------------------\n");
        System.out.println("REQ 3: PRINTING NODES AND THEIR CHILDREN");
        System.out.println("ROOT : " + tree.getRootKey());

        //tree representation
        for(int key: numbers)
            tree.printContent(key);

        System.out.println("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        System.out.println("REQ 4&5: delete non-leaf node and display");

        System.out.println("[ALL NON-LEAF NODES]");
        tree.printParents();
        //ASSUMING CORRECT INPUT
        Scanner input = new Scanner(System.in);
        System.out.println("Enter parent node to delete: ");
        int num = input.nextInt();

        tree.printContent(num);
        System.out.println("Deleting " + num);
        tree.delete(num);

        if(!tree.search(num)) {
            System.out.println(num + " is no longer in the tree!\nUPDATE TREE TRAVERSAL");
            tree.printInOrder();
        }

        System.out.println("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");

        System.out.println("REQ 6&7");
        int randNum = rand.nextInt(99) +1;
        System.out.println("Adding RANDOM NUMBER [" + randNum + "] to the tree.");
        tree.add(randNum);
        tree.printContent(randNum);
        tree.printInOrder();


    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                          BST CLASS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class BST {

    private Node root = null;
    int numNodes;

    public BST(int []list){
        System.out.println("REQ2: Adding elements to the tree...");
        numNodes = 0;
        for(int key: list)
            add(key);
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //user friendly
    public void add(int key){
        root = insert(root, key);
    }
    //BST operation
    private Node insert(Node root, int key){
        if(root == null) {
            numNodes++;
            return new Node(key);
        }

        if(key < root.key)
            root.left = insert(root.left, key);
        else if( key >= root.key)
            root.right = insert(root.right, key);
        else
            return root;

        return root;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //recursion until node is found
    //else return null
    private Node getNode(Node n, int key){
        if(n != null)
        {
            if(n.key == key)
                return n;
            else
            {
                if(key < n.key)
                    return getNode(n.left, key);
                else
                    return getNode(n.right, key);
            }
        }
        else
            return null;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    public boolean hasChildren(Node node){
        return (node.left != null || node.right != null);
    }

    public int getRootKey(){
        return (root != null) ? root.key : -1;
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    public void printContent(int key){
        Node parent = getNode(root, key);

        if(parent != null)
        {
            System.out.println("---------------");
            System.out.println("Parent:" + parent.key);

            if(parent.left != null)
                System.out.println("Left Child: " + parent.left.key);
            else
                System.out.println("Left Child: NULL");

            if(parent.right != null)
                System.out.println("Right Child: " + parent.right.key);
            else
                System.out.println("Right Child: NULL");

            System.out.println("---------------");
        }
        else
            System.out.println(key + " is not in the tree.");
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    public void printParents(){
        System.out.print("[");
        printParentsPrivate(root);
        System.out.print("]");
        System.out.println();
    }

    private void printParentsPrivate(Node root){
        if(root != null){

            if(hasChildren(root)) {
                System.out.print(root.key + ",");
                printParentsPrivate(root.left);
                printParentsPrivate(root.right);
            }
        }
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    public void printInOrder(){
        System.out.println("IN ORDER");
        System.out.print("[");
        printInOrderPrivate(root);
        System.out.print("]\n" + numNodes);
        System.out.println();
    }
    private void printInOrderPrivate(Node root){
        if(root != null){

            if(root.left != null)
                printInOrderPrivate(root.left);
            System.out.print(root.key + ",");
            if(root.right != null)
                printInOrderPrivate(root.right);
        }
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    public void delete(int key){
        root = deleteNode(key, root);
    }
    //recursion until node/key-target is found
    private Node deleteNode(int target, Node cursor){

        if (cursor.key ==target) {//found

            //determine if NODE has children

            //0 CHILDREN
            if(cursor.left == null && cursor.right == null) {
                numNodes--;
                return null;//node no longer has relation to the BST!
            }
            //HAS LEFT CHILD
            else if (cursor.right == null && cursor.left !=null) {
                return cursor.left;
                //"delete"
            }
            //HAS RIGHT CHILD
            else if (cursor.right != null && cursor.left == null) {
                return cursor.right;//"delete"
            }
            else{
                //TWO CHILDREN
                //swapping with PREDECESSOR for this program
                int predecessor = getLargest(cursor.left);
                cursor.key = predecessor;
                cursor.left = deleteNode(predecessor, cursor.left);
                return cursor;
            }
        }
        //traverse through tree until KEY TARGET is found
        if (target < cursor.key) {//left subtree
            cursor.left = deleteNode(target, cursor.left);
            return cursor;
        }
        else//right subtree
            cursor.right = deleteNode(target, cursor.right);
        return cursor;
    }

    //IN ORDER PREDECESSOR
    private int getLargest(Node n){
        return n.right == null ? n.key : getLargest(n.right);
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    public boolean search(int key){
        return Contains(root, key);
    }
    private boolean Contains(Node root, int key){
        if(root == null)
            return false;
        if(root.key == key)
            return true;
        if(root.key > key)
            return Contains(root.left, key);
        return Contains(root.right, key);
    }
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//                          NODE CLASS
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class Node{
    int key;
    Node left, right;

    public Node(int key){
        this.key = key;
        left = null;
        right = null;
    }
    public int getData(){
        return this.key;
    }
}
