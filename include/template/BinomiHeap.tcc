/* Gruppe :
 * 
 * Lars Siggelkow    157 721
 * Ronald Kluth      158 561
 * Valentin Ziegler  157 500
 *
 */

#ifndef BIN_HEAP_HH__
#  error "This file is included by BinomiHeap.hh"
#  error "manual inclusion is nonsense"
#endif BIN_HEAP_HH__

/******************************************************
 *
 * Implementation binaerer Heap mit fester Groesse
 * nach Definition der Klasse BinomiHeap in BinomiHeap.hh
 *
 ******************************************************/

// Constructor. [maximal_size] legt Groesse des Heaps fest
template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: BinomiHeap (int maximal_size)
{
  max_size = maximal_size;
  cur_size = 0;

  real_node_map = new (Node*) [max_size];  // Knotenvektor alloziieren.
  node_map = real_node_map - 1;            // Pseudo-Vektor initialisieren

  id_map = new int [max_size];             // Assoziatives array initialisieren
  for (int i = 0; i < maximal_size; i++)
    id_map [i] = -1;                       // -1 bedeutet ID nicht enthalten
}


// Destructor
template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: ~BinomiHeap ()
{ // Speicher freigeben
  for (int i = 0; i < cur_size; i++)
    delete real_node_map [i];
  delete [] real_node_map;
  delete [] id_map;
}




// Element [item] mit Schluessel [key] einfuegen. Zugriffs-ID [id].
// Bei Verwendung von BinomiHeap_Checking:
// wirft InvalidIdException, falls id nicht innerhalb [ 0, Size )
// wirft DuplicateIdException, falls id bereits in Verwendung
template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
void BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: Insert (ITEM_T item, KEY_T key, int id)
{
  // Heap voll ?
  RANGE_CHECKING :: CheckFull (cur_size, max_size);

  cur_size++;

  // am Ende des Heaps einfuegen
  int pos = cur_size;
  InitNode (pos, item, key, id); 

  // Element versickern
  while (pos > 1 && node_map [MotherIndex (pos)] -> key > key)
    {
      ExchangeNodes (pos, MotherIndex (pos));
      pos = MotherIndex (pos);
    }
}



// Element mit minimalem Key zurueckliefern und aus Heap entfernen.
// Bei Verwendung von BinomiHeap_Checking:
// wirft HeapEmptyException, falls Heap leer.
template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
ITEM_T BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> ::  ExtractMin ()
{
  // Heap leer ?
  RANGE_CHECKING :: CheckEmpty (cur_size);

  Node* t_node = node_map [1];
  ITEM_T r_item = t_node -> item;

  // letztes Element an Wurzel einfuegen
  node_map [1] = node_map [cur_size --];
  id_map [node_map [1] -> id] = 1;

  // "Hochblubbern" der neuen Wurzel
  int pos = 1;
  KEY_T key = node_map [1] -> key;

  while (node_map [SmallestChildIndex (pos)] -> key < key)
    {
      int s_pos = SmallestChildIndex (pos);
      ExchangeNodes (pos, s_pos);
      pos = s_pos;
    }

  // Aufraeumen und min. Element zurueckgeben.
  id_map [t_node -> id] = -1;
  delete t_node;
  return r_item;
}


// liefert (statische) Groesse des Heaps zurueck
template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
inline int BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: Size ()
{
  return max_size;
}

// liefert momentane Anzahl enthaltener Elemente zurueck
template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
inline int BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: CurSize ()
{
  return cur_size;
}


// Heap leer ?
template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
inline bool BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: Empty ()
{
  return cur_size == 0;
}


// Schluessel des Elementes mit ID [id] erniedrigen
// Bei Verwendung von BinomiHeap_Checking:
// wirft InvalidKeyException, falls neuer Schluessel groesser als alter
template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
void BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: DecreaseKey (KEY_T new_key, int id)
{
  // Sicherstellen, dass Element in Heap
  CheckId (id);

  int pos = id_map [id];

  // Sicherstellen, dass neuer Key <= alter Key
  RANGE_CHECKING :: CheckDecrease (node_map [pos] -> key, new_key);

  node_map [pos] -> key = new_key;

  // Versickern
  while (pos > 1 && node_map [MotherIndex (pos)] -> key > new_key)
    {
      ExchangeNodes (pos, MotherIndex (pos));
      pos = MotherIndex (pos);
    }
}



// Aktueller Schluessel des Elementes mit ID [id]
// Bei Verwendung von BinomiHeap_Checking:
// wirft InvalidIdException
template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
KEY_T BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: GetKey (int id)
{
  CheckId (id);
  return node_map [id_map [id]] -> key;
}


// Element der ID [id]
// Bei Verwendung von BinomiHeap_Checking:
// wirft InvalidIdException
template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
ITEM_T BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: GetItem (int id)
{
  CheckId (id);
  return node_map [id_map [id]] -> item;
}

template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
inline void BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: ExchangeNodes (int i, int j)
{
  // Knoten im Vektor vertauschen
  Node* dummy = node_map [i];
  node_map [i] = node_map [j];
  node_map [j] = dummy;

  // assoziatives Array angleichen
  id_map [node_map [i] -> id] = i;
  id_map [node_map [j] -> id] = j;
}

template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
inline void BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: InitNode (int i, ITEM_T item, KEY_T key, int id)
{
  // Gueltige ID ?
  RANGE_CHECKING :: CheckIDRange (id, max_size);

  // ID schon benutzt ?
  RANGE_CHECKING :: CheckDuplicateID (id_map [id]);

  // assoziatives Array anpassen
  id_map [id] = i;

  // Knoten einfuegen
  node_map [i] = new Node;
  node_map [i] -> item = item;
  node_map [i] -> key = key;
  node_map [i] -> id = id;
}





template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
inline int BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: LeftChildIndex (int index)
{ // Kind 1 an Pos [index * 2]
  return index << 1;
}





template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
inline int BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: RightChildIndex (int index)
{ // Kind 2 an Pos [(index * 2) + 1]
  return (index << 1) + 1; 
}




template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
inline int BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: SmallestChildIndex (int index)
{ 
  int l = LeftChildIndex (index);
  int r = RightChildIndex (index);

  if (l > cur_size)
    return index;   // Knoten kann keine Kinder haben
  else if (l == cur_size)
    return l;       // Knoten kann nur ein Kind haben.
  else
    return (node_map [l] -> key < node_map [r] -> key) ? l : r;
}




template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
inline int BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: MotherIndex (int index)
{ // Mutter an Pos [index / 2]
  return index >> 1;
}


// Bei Verwendung von BinomiHeap_NoChecking wird diese Methode vom Compiler
// wegoptimiert 
template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
inline void BinomiHeap <ITEM_T, KEY_T, RANGE_CHECKING> :: CheckId (int id)
{
  // Id ausserhalb Intervall [ 0, Size ) ?
  RANGE_CHECKING :: CheckIDRange (id, max_size);

  // Kein Element mit dieser Id im Heap ?
  RANGE_CHECKING :: CheckId (id_map [id]);
}


// Implemetation des RangeCheckings

// Neutral, falls noch Platz im Heap.
// wirft sonst HeapFullException. 
template <typename KEY_T>
inline void BinomiHeap_Checking <KEY_T> :: CheckFull (int heap_cur_size, int heap_max_size)
{
  if (heap_cur_size >= heap_max_size)
    throw HeapFullException ();
}
  
// Neutral, falls noch Elemente im Heap.
// wirft sonst HeapEmptyException. 
template <typename KEY_T>
inline void BinomiHeap_Checking <KEY_T> :: CheckEmpty (int heap_cur_size)
{
  if (heap_cur_size == 0)
    throw HeapEmptyException ();
}

// Sicherstellen, dass neuer Key <= alter Key
// wirft sonst InvalidKeyException.
template <typename KEY_T>
inline void BinomiHeap_Checking <KEY_T> :: CheckDecrease (KEY_T old_key, KEY_T new_key)
{
  if (old_key < new_key)
    throw InvalidKeyException ();
}

// Sicherstellen, dass id in gueltigen Grenzen ist.
// wirft ansonsten InvalidIdException.
template <typename KEY_T>
inline void BinomiHeap_Checking <KEY_T> :: CheckIDRange (int id, int heap_max_size)
{
  if (id < 0 || id >= heap_max_size)
    throw InvalidIdException ();
}

// Sichherstellen, dass id unbenuzt.
//  wirft ansonsten DuplicateIdException. 
template <typename KEY_T>
inline void BinomiHeap_Checking <KEY_T> :: CheckDuplicateID (int id_val)
{
  if (id_val != -1)
    throw DuplicateIdException ();
}

// Neutral, falls [id] gueltige ID eines im Heap
// enthaltenen Elementes ist.
// wirft ansonsten InvalidIdException
template <typename KEY_T>
inline void BinomiHeap_Checking <KEY_T> :: CheckId (int id_val)
{
  if (id_val == -1)
    throw InvalidIdException ();
}

