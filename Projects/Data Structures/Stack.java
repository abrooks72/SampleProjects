/*  Program 2
	Austin Brooks
    cssc0871
*/

package data_structures;

import java.util.Iterator;
import java.util.NoSuchElementException;
import java.util.ConcurrentModificationException;

public class Stack<E extends Comparable<E>> implements Iterable<E>
{
	private LinearListADT<E> list;
	
	public Stack()
	{
		list = new LinearList<E>();
	}
	
	public void push(E obj)
	{
		list.addFirst(obj);
	}
	
	public E pop()
	{
		return list.removeFirst();
	}
	
	public int size()
	{
		return list.size();
	}
	
	public boolean isEmpty()
	{
		return list.isEmpty();
	}
	
	public E peek()
	{
		return list.peekFirst();
	}
	
	public boolean contains(E obj)
	{
		return list.contains(obj);
	}
	
	public void makeEmpty()
	{
		list.clear();
	}
	
	public boolean remove(E obj)
	{
		return (list.remove(obj) != null);
	}
	
	public Iterator<E> iterator()
	{
		return list.iterator();
	}
}