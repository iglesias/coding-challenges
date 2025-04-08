#[derive(Clone, Debug, Eq, PartialEq)]
pub struct ListNode {
    pub val: i32,
    pub next: Option<Box<ListNode>>
}

impl ListNode {
    #[inline]
    fn new(val: i32) -> Self {
        ListNode {
            next: None,
            val
        }
    }
}

#[derive(Debug, Eq, PartialEq)]
pub struct TreeNode {
    pub val: i32,
    pub left:  Option<Rc<RefCell<TreeNode>>>,
    pub right: Option<Rc<RefCell<TreeNode>>>,
}

impl TreeNode {
    #[inline]
    pub fn new(val: i32, left: Option<Rc<RefCell<TreeNode>>>, right: Option<Rc<RefCell<TreeNode>>>) -> Self {
        TreeNode {
            val,
            left,
            right
        }
    }
}

use std::rc::Rc;
use std::cell::RefCell;

pub fn is_sub_path(head: Option<Box<ListNode>>, root: Option<Rc<RefCell<TreeNode>>>) -> bool {
    is_sub_path_internal(head.as_ref(), root.as_ref())
}

fn is_sub_path_internal(head: Option<&Box<ListNode>>, root: Option<&Rc<RefCell<TreeNode>>>) -> bool {
    if root.is_none() {
        return false;
    }
    
    let root_ref = root.unwrap().borrow();
    
    is_sub_path_impl(head, root) ||
    is_sub_path_internal(head, root_ref.left.as_ref()) ||
    is_sub_path_internal(head, root_ref.right.as_ref())
}

fn is_sub_path_impl(list_node: Option<&Box<ListNode>>, tree_node: Option<&Rc<RefCell<TreeNode>>>) -> bool {
    if list_node.is_none() {
        return true;
    }
    if tree_node.is_none() {
        return false;
    }
    
    let list_node = list_node.unwrap();
    let tree_node_ref = tree_node.unwrap().borrow();

    if list_node.val == tree_node_ref.val {
        is_sub_path_impl(list_node.next.as_ref(), tree_node_ref.left.as_ref()) || 
        is_sub_path_impl(list_node.next.as_ref(), tree_node_ref.right.as_ref())
    } else {
        false
    }
}

fn main() {
    let mut list = ListNode::new(2);
    list.next = Some(Box::new(ListNode::new(2)));
    list.next.as_mut().unwrap().next = Some(Box::new(ListNode::new(1)));

    let leaf = TreeNode::new(1, None,                              None);
    let l1   = TreeNode::new(2, Some(Rc::new(RefCell::new(leaf))), None);
    let l2   = TreeNode::new(2, None,                              Some(Rc::new(RefCell::new(l1))));
    let tree = TreeNode::new(2, Some(Rc::new(RefCell::new(l2))),   None);

    assert!(is_sub_path(Some(Box::new(list)), Some(Rc::new(RefCell::new(tree)))))
}
