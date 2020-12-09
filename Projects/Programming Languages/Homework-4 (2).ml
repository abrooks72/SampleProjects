(* CS320 Programming Languages
   Homework-4
   Due December 5 (Thursday) 11:59PM
   Total Points: 100
*)

(* Please implement the pre-declared function in each problem.
   For each problem, I give several test cases to help you
   check whether your implementation is correct. If your code
   is correct, the value of every test case should be TRUE.
   You may also come up with your own test cases.

   NOTE that the given test cases may NOT be completed, which means
   you may NOT get full credits even if your code passes all the
   given test cases. To get full credits, your code should cover
   ALL POSSIBLE scenarios. Please test your code with test cases
   that cover all possible scenarios (you may test your code in
   your local .ml files and copy your implementation back to this
   file).

   I will write a AUTO-GRADER to grade this homework. In order
   to ensure that the auto-grader can READ your submission,
   please follow the following rules:
   (1) Directly modify this file by inserting your code and
   submit the modified ONE SINGLE .ml file in Blackboard (no hardcopy).
   (2) ONLY write your code between line (* Your code START HERE *)
   and line (* Your code END HERE *). Please delete the DEFAULT
   return between these two lines (I add the default return to
   just ensure NO compile errors).
   (3) You may define auxiliary functions for each problem.
   If you do so, please ONLY define your auxiliary functions
   between line (* Auxiliary functions (if defined) START HERE *)
   and line (* Auxiliary functions (if defined) END HERE *).
   In addition, please do NOT use the same name with any
   pre-declared function in this homework.
   (4) MAKE SURE that your submission is compilable (with
   the ocamlc command in terminals) and does NOT have any
   compile errors. If your submission CANNOT be compiled,
   you will get ZERO for this homework.
*)



(* General forms of MAP and REDUCE functions, which are
   used in Problem 11-14. *)
let rec map (f:'a -> 'b) (xs: 'a list) : 'b list =
  match xs with
    | [] -> []
    | hd::tl -> (f hd) :: (map f tl);;

let rec reduce (f:'a -> 'b -> 'b) (u:'b) (xs:'a list) : 'b =
  match xs with
    | [] -> u
    | hd::tl -> f hd (reduce f u tl);;




(* Problem 1 [5 points]. Write a function that returns
   the volume of a cylinder with height h, radius r.
   Use the pre-declared pi (i.e., let pi = 3.14;;)
*)
let pi = 3.14;;
let vol (h:float) (r:float) : float =
  (* Your code START HERE *)
  pi *. r *. r *. h
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)
((vol 2.0 3.0) = 56.519999999999996) || ((vol 2.0 3.0) = 56.52);;

print_string "\n====Test of Problem 1====\n";;
print_string ("The result of \"(vol 2.0 3.0) = 56.52\" is " ^ (string_of_bool (((vol 2.0 3.0) = 56.519999999999996) || ((vol 2.0 3.0) = 56.52))) ^ ".\n");;



(* Problem 2 [5 points]. Write a function that takes two numbers
   and returns the positive difference between these two numbers.
   The function ALWAYS returns a NON-NEGATIVE number.*)
let diff (x:int) (y:int) : int =
  (* Your code START HERE *)
  abs (x - y)
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)
(diff 5 3) = 2;;
(diff 3 5) = 2;;
(diff 1 1) = 0;;

print_string "\n====Test of Problem 2====\n";;
print_string ("The result of \"(diff 5 3) = 2\" is " ^ (string_of_bool ((diff 5 3) = 2)) ^ ".\n");;
print_string ("The result of \"(diff 3 5) = 2\" is " ^ (string_of_bool ((diff 3 5) = 2)) ^ ".\n");;
print_string ("The result of \"(diff 1 1) = 0\" is " ^ (string_of_bool ((diff 1 1) = 0)) ^ ".\n");;



(* Problem 3 [5 points]. Write a function that takes a number and
   returns true if it is odd otherwise returns false. 
   Note that negative integers are neither odd nor even.*)
let odd (x:int) : bool =
  (* Your code START HERE *)
  (* Check for positive number and then check for divisibility by 2 *)
if (x > 0) && (x mod 2 != 0) then
true
(* If condition fails *)
else
false
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)
(odd 5) = true;;
(odd 4) = false;;
(odd (-2)) = false;;
(odd (-1)) = false;;

print_string "\n====Test of Problem 3====\n";;
print_string ("The result of \"(odd 5) = true\" is " ^ (string_of_bool ((odd 5) = true)) ^ ".\n");;
print_string ("The result of \"(odd 4) = false\" is " ^ (string_of_bool ((odd 4) = false)) ^ ".\n");;
print_string ("The result of \"(odd (-2)) = false\" is " ^ (string_of_bool ((odd (-2)) = false)) ^ ".\n");;
print_string ("The result of \"(odd (-1)) = false\" is " ^ (string_of_bool ((odd (-1)) = false)) ^ ".\n");;

(* Problem 4 [10 points]. Suppose we pre-define a struct
   type student = {name:string; id:int; score:float},
   write functions that make a string*int*float tuple
   into an student and vice versa. *)
type student = {name:string; id:int; score:float};;
let tuple_to_stu (n:string) (i:int) (s:float) : student =
  (* Your code START HERE *)
  {name = n; id = i; score = s}
  (* Your code END HERE *)
;;
let stu_to_tuple (stu:student) : (string*int*float) =
  (* Your code START HERE *)
  stu.name , stu.id , stu.score
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)

let stu = tuple_to_stu "John" 34 95.0;;
stu = ({name="John"; id=34; score=95.0});;
(stu_to_tuple stu) = ("John", 34, 95.0);;

print_string "\n====Test of Problem 4====\n";;
print_string ("The result of \"stu = ({name=\"John\"; id=34; score=95.0})\" is " ^ (string_of_bool (stu = ({name="John"; id=34; score=95.0}))) ^ ".\n");;
print_string ("The result of \"(stu_to_tuple stu) = (\"John\", 34, 95.0)\" is " ^ (string_of_bool ((stu_to_tuple stu) = ("John", 34, 95.0))) ^ ".\n");;



(* Problem 5 [5 points]. Write a recursive function that
   takes a number and returns factorial of that number.
   Suppose the factorial of a negative number is -1. *)
let rec fac (x:int) : int =
  (* Your code START HERE *)
  if x < 0 then -1
  else if x = 0 then 1
  else x * fac(x - 1);;
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)
(fac 3) = 6;;
(fac 0) = 1;;
(fac (-2)) = -1;;

print_string "\n====Test of Problem 5====\n";;
print_string ("The result of \"(fac 3) = 6\" is " ^ (string_of_bool ((fac 3) = 6)) ^ ".\n");;
print_string ("The result of \"(fac 0) = 1\" is " ^ (string_of_bool ((fac 0) = 1)) ^ ".\n");;
print_string ("The result of \"(fac (-2)) = -1\" is " ^ (string_of_bool ((fac (-2)) = -1)) ^ ".\n");;



(* Problem 6 [15 points]. Write functions that
   takes an integer list and sorts data with
   the selection sort algorithm.

   You may define auxiliary functions that are
   called by the sort function. If you define
   auxiliary functions, please do NOT use the same
   name with any pre-declared function in
   this homework.

   Please refer to the following link for
   the selection sort.
   https://www.geeksforgeeks.org/selection-sort/
*)

(* Auxiliary functions (if defined) START HERE *)
 let rec select x = function
    | [] -> x, []
    | h::t ->
        let x, h = if x<h then x, h else h, x in
        let x, t = select x t in
        x, h::t;;
(* Auxiliary functions (if defined) END HERE *)

let rec sort (xs:int list) : int list =
  (* Your code START HERE *)
  match xs with
  | [] -> []
  | h::t -> match select h t with
  | h, t -> h::sort t
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)
(sort [9;5;4;3;6]) = [3;4;5;6;9];;
(sort []) = [];;

print_string "\n====Test of Problem 6====\n";;
print_string ("The result of \"(sort [9;5;4;3;6]) = [3;4;5;6;9]\" is " ^ (string_of_bool ((sort [9;5;4;3;6]) = [3;4;5;6;9])) ^ ".\n");;
print_string ("The result of \"(sort []) = []\" is " ^ (string_of_bool ((sort []) = [])) ^ ".\n");;



(* Problem 7 [5 points]. Write a function to find the
   Fibonacci number of given interger.
   Suppose the Fibonacci number of a negative number is -1.

   Please refer to the Wikipedia
   (https://en.wikipedia.org/wiki/Fibonacci_number)
   for the Fibonacci number. *)
let rec fib (n:int) : int =
  (* Your code START HERE *)
  if n < 1 then
    -1
  else if n < 3 then
     1
  else
    fib (n-1) + fib (n-2)
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)
(fib 3) = 2;;
(fib 6) = 8;;
(fib (-5)) = -1;;

print_string "\n====Test of Problem 7====\n";;
print_string ("The result of \"(fib 3) = 2\" is " ^ (string_of_bool ((fib 3) = 2)) ^ ".\n");;
print_string ("The result of \"(fib 6) = 8\" is " ^ (string_of_bool ((fib 6) = 8)) ^ ".\n");;
print_string ("The result of \"(fib (-5)) = -1\" is " ^ (string_of_bool ((fib (-5)) = -1)) ^ ".\n");;



(* Problem 8 [8 points]. Write a function that takes
   an integer list and counts odd numbers in the list.
   You may call the odd function implemented in Problem 3
   to check whether a given number is odd.
*)
let rec odd_num (xs:int list) : int =
  (* Your code START HERE *)
  match xs with
    | [] -> 0 (* If list is empty *)
    | head::body ->
    if (head mod 2 = 1) = true then (1 + odd_num body)
    else odd_num body
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)
(odd_num [-1;1;2;3;4;5]) = 3;;

print_string "\n====Test of Problem 8====\n";;
print_string ("The result of \"(odd_num [-1;1;2;3;4;5]) = 3\" is " ^ (string_of_bool ((odd_num [-1;1;2;3;4;5]) = 3)) ^ ".\n");;



(* Problem 9 [8 points]. Write a function that takes
   an integer list and returns the sum of all elements in the list.
   If the list is empty, then return None. *)
let rec sum (xs:int list) : int option =
  (* Your code START HERE *)
  let rec sum_helper(xs: int list) : int =
  match xs with
  | [] -> 0
  | head::tail -> head + sum_helper(tail) in
  match xs with
  | [] -> None
  | head::tail -> Some(head + sum_helper(tail))
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)
(sum [1;2;3]) = Some 6;;
(sum []) = None;;

print_string "\n====Test of Problem 9====\n";;
print_string ("The result of \"(sum [1;2;3]) = Some 6\" is " ^ (string_of_bool ((sum [1;2;3]) = Some 6)) ^ ".\n");;
print_string ("The result of \"(sum []) = None\" is " ^ (string_of_bool ((sum []) = None)) ^ ".\n");;



(* Problem 10 [8 points]. Write a function that takes
   a int*string list. The function produces another
   list of string*int using the elements of the given list. *)
type intstr = int*string;;
type strint = string*int;;
let rec swap (xs:intstr list) : strint list=
  (* Your code START HERE *)
   match xs with
  | [] -> []
  | (first,second)::tail -> (second,first)::swap(tail)
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)
(swap [(1,"ab"); (2,"cd")]) = [("ab",1); ("cd",2)];;
(swap []) = [];;

print_string "\n====Test of Problem 10====\n";;
print_string ("The result of \"(swap [(1,\"ab\"); (2,\"cd\")]) = [(\"ab\",1); (\"cd\",2)]\" is " ^ (string_of_bool ((swap [(1,"ab"); (2,"cd")]) = [("ab",1); ("cd",2)])) ^ ".\n");;
print_string ("The result of \"(swap []) = []\" is " ^ (string_of_bool ((swap []) = [])) ^ ".\n");;



(* Problem 11 [5 points] Use MAP to write a function that
   takes an int list and an int and multiplies every
   entry in the list by the int.

   You may define auxiliary functions that are
   called by the times_x function. If you define
   auxiliary functions, please do NOT use the same
   name with any pre-declared function in
   this homework. *)

(* Auxiliary functions (if defined) START HERE *)
(* Auxiliary functions (if defined) END HERE *)

let times_x (x: int) (lst: int list) : int list = 
  (* Your code START HERE *)
  List.map (fun i -> x * i) lst
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)
(times_x 5 [1;2;3]) = [5;10;15];;
(times_x 5 []) = [];;
(times_x 2 [1;2;3]) = [2;4;6];;

print_string "\n====Test of Problem 11====\n";;
print_string ("The result of \"(times_x 5 [1;2;3]) = [5;10;15]\" is " ^ (string_of_bool ((times_x 5 [1;2;3]) = [5;10;15])) ^ ".\n");;
print_string ("The result of \"(times_x 5 []) = []\" is " ^ (string_of_bool ((times_x 5 []) = [])) ^ ".\n");;
print_string ("The result of \"(times_x 2 [1;2;3]) = [2;4;6]\" is " ^ (string_of_bool ((times_x 2 [1;2;3]) = [2;4;6])) ^ ".\n");;



(* Problem 12 [8 points] Use MAP to write a function that
   takes an int list and produces a "multiplication table",
   which is a list of int lists showing the product of any
   two entries in the input int list.
   e.g. mult_table [1;2;3] => [[1; 2; 3]; [2; 4; 6]; [3; 6; 9]]

   You may call the times_x function implemented in Problem 11.

   You may define auxiliary functions that are
   called by the mult_table function. If you define
   auxiliary functions, please do NOT use the same
   name with any pre-declared function in
   this homework. *)

(* Auxiliary functions (if defined) START HERE *)
let times_x (x: int) (lst: int list) : int list = 
  (* Your code START HERE *)
  List.map (fun i -> x * i) lst
  (* Your code END HERE *)
;;
(* Auxiliary functions (if defined) END HERE *)

let mult_table (lst: int list) : int list list =
  (* Your code START HERE *)
  let rec mult_each (is : int list) : int list list =
   match is with
   | [] -> []
   | head :: tail -> times_x head lst :: mult_each tail
   in mult_each lst
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)
(mult_table [1;2;3]) = [[1;2;3]; [2;4;6]; [3;6;9]];;
(mult_table [1;2;3;4]) = [[1;2;3;4]; [2;4;6;8]; [3;6;9;12]; [4;8;12;16]];;
(mult_table []) = [];;

print_string "\n====Test of Problem 12====\n";;
print_string ("The result of \"(mult_table [1;2;3]) = [[1;2;3]; [2;4;6]; [3;6;9]]\" is " ^ (string_of_bool ((mult_table [1;2;3]) = [[1;2;3]; [2;4;6]; [3;6;9]])) ^ ".\n");;
print_string ("The result of \"(mult_table [1;2;3;4]) = [[1;2;3;4]; [2;4;6;8]; [3;6;9;12]; [4;8;12;16]]\" is " ^ (string_of_bool ((mult_table [1;2;3;4]) = [[1;2;3;4]; [2;4;6;8]; [3;6;9;12]; [4;8;12;16]])) ^ ".\n");;
print_string ("The result of \"(mult_table []) = []\" is " ^ (string_of_bool ((mult_table []) = [])) ^ ".\n");;



(* Problem 13 [5 points] Use REDUCE to write a function that
   takes a list of boolean values [x1; x2; ... ; xn] and
   returns x1 AND x2 AND ... AND xn.
   For simplicity, assume and_list [] returns TRUE.

   You may define auxiliary functions that are
   called by the and_list function. If you define
   auxiliary functions, please do NOT use the same
   name with any pre-declared function in
   this homework. *)

(* Auxiliary functions (if defined) START HERE *)
(* Auxiliary functions (if defined) END HERE *)

let and_list (lst: bool list) : bool = 
  (* Your code START HERE *)
  List.fold_left (&&) true lst
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)
(and_list [true]) = true;;
(and_list [false; true]) = false;;
(and_list [false; true; false]) = false;;
(and_list []) = true;;

print_string "\n====Test of Problem 13====\n";;
print_string ("The result of \"(and_list [true]) = true\" is " ^ (string_of_bool ((and_list [true]) = true)) ^ ".\n");;
print_string ("The result of \"(and_list [false; true]) = false\" is " ^ (string_of_bool ((and_list [false; true]) = false)) ^ ".\n");;
print_string ("The result of \"(and_list [false; true; false]) = false\" is " ^ (string_of_bool ((and_list [false; true; false]) = false)) ^ ".\n");;
print_string ("The result of \"(and_list []) = true\" is " ^ (string_of_bool ((and_list []) = true)) ^ ".\n");;



(* Problem 14 [8 points] Use REDUCE to write a function that
   takes an int list and returns the min and max of the list.
   If the input list is empty, return (None, None).

   You may define auxiliary functions that are
   called by the bounds function. If you define
   auxiliary functions, please do NOT use the same
   name with any pre-declared function in
   this homework. *)

(* Auxiliary functions (if defined) START HERE *)
(* Auxiliary functions (if defined) END HERE *)

let bounds (lst:int list) : (int option * int option) = 
  (* Your code START HERE *)
  match lst with
  | [] -> (None, None)
  | head :: tail -> (Some (List.fold_left min head tail), Some (List.fold_left max head tail))
  (* Your code END HERE *)
;;

(* test case: all results should be TRUE *)
(bounds [1;2;3]) = (Some 1, Some 3);;
(bounds [-1;-2;-3;1;2;3]) = (Some (-3), Some 3);;
(bounds []) = (None, None);;

print_string "\n====Test of Problem 14====\n";;
print_string ("The result of \"(bounds [1;2;3]) = (Some 1, Some 3)\" is " ^ (string_of_bool ((bounds [1;2;3]) = (Some 1, Some 3))) ^ ".\n");;
print_string ("The result of \"(bounds [-1;-2;-3;1;2;3]) = (Some (-3), Some 3)\" is " ^ (string_of_bool ((bounds [-1;-2;-3;1;2;3]) = (Some (-3), Some 3))) ^ ".\n");;
print_string ("The result of \"(bounds []) = (None, None)\" is " ^ (string_of_bool ((bounds []) = (None, None))) ^ ".\n");;

