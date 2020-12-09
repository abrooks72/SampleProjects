package data_structures;

public class Hashtable<K,V> implements DictionaryADT<K,V>
{
	private int currentSize;
	private int maxSize;
	private int tableSize;
	private long modCounter;
	private LinearList<DictionaryNode<K,V>> [] list;
	
	class DictionaryNode<K,V> implements Comparable<DictionaryNode<K,V>>
	{
		K key;
		V value;
		
		public DictionaryNode(K k, V v)
		{
			key = k;
			value = v;
		}
		
		public int compareTo(DictionaryNode<K,V> node)
		{
			return ((Comparable<K>)key).compareTo((K)node.key);
		}
	}
}

/*
RUN OUTPUT:

*/