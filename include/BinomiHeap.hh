
#ifndef BIN_HEAP_HH
#define BIN_HEAP_HH

/************************************************************
 *
 * Ein binaerer Heap mit statischer Groesse [Size ()]
 * welche initial festzulegen ist.
 *
 * Schablonenklasse mit Parametern
 *
 * ITEM_T         :   Typ des Elementes
 * KEY_T          :   Typ der Schluessel
 * RANGE_CHECKING :   entweder BinomiHeap_Checking, um auf
 *                             ungueltige ID's, Schluesselwerte
 *                             etc testen lassen
 *                    oder     BinomiHeap_NoChecking; Keine Tests,
 *                             keine Exceptions, dafuer effizientes
 *                             Laufzeitverhalten
 *                    oder     Selbstimplementiere Klasse, welche
 *                             gleichnamige Methoden exportiert.
 *
 * Bei komplexem Elementtyp sollte aus Effizienzgruenden ein
 * Pointer als ITEM_T verwendet werden.
 * 
 * Folgende naive / ueberladene Operatoren muessen von KEY_T
 * bereitgestellt werden:
 *
 * operator <
 * operator >
 * operator =
 * KEY_T (const KEY_T&)
 *
 * Fuer Zugriffe auf Elemente im Heap wird eine numerische ID
 * vom typen int aus dem intervall [ 0, Size ) verwendet.
 * 
 ***********************************************************/

// Exceptions

// werden geworfen, falls als Rangechecker BinomiHeap_Exceptions verwended wird

class InvalidIdException {};      // ungueltige ID in uebergebenem Parameter
class DuplicateIdException {};    // ID zum zweiten Male eingefuegt 
class HeapEmptyException {};      // Heap ist leer (bei ExtractMin)
class HeapFullException {};       // Heap ist voll (bei Insert)
class InvalidKeyException {};     // neuer Schluessel groesser als alter (bei DecreaseKey) 



template <typename ITEM_T, typename KEY_T, typename RANGE_CHECKING>
class BinomiHeap
{
public:

  BinomiHeap (int maximal_size);    // Constructor. [maximal_size] legt Groesse des Heaps fest
  ~BinomiHeap ();

  /************** oeffentliche Heapoperationen *******************/

  // Element [item] mit Schluessel [key] einfuegen. Zugriffs-ID [id].
  // Bei Verwendung von BinomiHeap_Checking:
  // wirft InvalidIdException, falls id nicht innerhalb [ 0, Size )
  // wirft DuplicateIdException, falls id bereits in Verwendung
  void Insert (ITEM_T item, KEY_T key, int id);

  // Element mit minimalem Key zurueckliefern und aus Heap entfernen
  // Bei Verwendung von BinomiHeap_Checking:
  // wirft HeapEmptyException, falls Heap leer.
  ITEM_T ExtractMin ();

  // Schluessel des Elementes mit ID [id] erniedrigen
  // Bei Verwendung von BinomiHeap_Checking:
  // wirft InvalidKeyException, falls neuer Schluessel groesser als alter
  void DecreaseKey (KEY_T new_key, int id);

  //********************* Informationen **************************/

  // liefert (statische) Groesse des Heaps zurueck
  inline int Size ();

  // liefert momentane Anzahl enthaltener Elemente zurueck
  inline int CurSize ();

  // Heap leer?
  inline bool Empty ();

  // aktueller Schluessel des Elementes mit ID [id]
  // Bei Verwendung von BinomiHeap_Checking:
  // wirft InvalidIdException
  KEY_T GetKey (int id);

  // Element der ID [id]
  // Bei Verwendung von BinomiHeap_Checking:
  // wirft InvalidIdException
  ITEM_T GetItem (int id);

  // ************************************************************/

private:

  class Node
  { // repraesentiert Knotenpunkt des Heap
  public:
    ITEM_T item;
    KEY_T key;
    int id;
  };

  int max_size;           // maximale Anzahl von Elementen
  int cur_size;           // aktuelle Anzahl von Elementen

  Node** real_node_map;   // Vektor, der alle Knoten enthaelt
  Node** node_map;        // Pseudo-vektor, mit dem gearbeitet wird,
                          // so dass real_node_map [i] == node_map [i+1]
                          // d.h. Indizes von node_map laufen von 1 bis n

  int* id_map;            // assoziatives array fuer ID's :
                          // node_map [ id_map [id] ] enthaelt
                          // Element mit ID [id]

  /****************** interne Operationen **********************
   *
   * kleine Hilfsoperationen, die aus Effizienzgruenden inline
   * deklariert werden
   *
   *************************************************************/

  // Vertauscht Knoten der Indizies i und j incl. Anpassung der id_map
  inline void ExchangeNodes (int i, int j);
  // Initiiert Knoten mit Index i inc. Eintrag in die id_map
  inline void InitNode (int i, ITEM_T item, KEY_T key, int id);

  // Liefert die Indizes der Kinder
  inline int LeftChildIndex (int index);
  inline int RightChildIndex (int index);

  // Liefert Index des Kindes mit dem kleinstem Schluessel
  // falls aufgrund eines hohen Index' keine Kinder im Heap
  // sein koennen, wird [index] zurueckgegeben
  inline int SmallestChildIndex (int index);

  // Index des Mutterknotens, liefert 0 falls
  // [index] bereits Wurzel
  inline int MotherIndex (int index);

  // Neutral, falls [id] gueltige ID eines im Heap
  // enthaltenen Elementes ist.
  // wirft ansonsten InvalidIdException
  inline void CheckId (int id);
};


// RangeCheckers for Binomi Heap

template <typename KEY_T>
class BinomiHeap_Checking
{
public:
  // Diese Kasse implementiert das Rangechecking fur den BinomiHeap.
  static inline void CheckFull (int heap_cur_size, int heap_max_size);
  static inline void CheckEmpty (int heap_cur_size);
  static inline void CheckDecrease (KEY_T old_key, KEY_T new_key);
  static inline void CheckIDRange (int id, int heap_max_size);
  static inline void CheckDuplicateID (int id_val);
  static inline void CheckId (int id_val);
};

template <typename KEY_T>
class BinomiHeap_NoChecking
{
public:
  // Diese Klasse ist neutral, alle Methodenaufrufe werdem vom Compiler wegoptimiert.
  static inline void CheckFull (int heap_cur_size, int heap_max_size) {};
  static inline void CheckEmpty (int heap_cur_size) {};
  static inline void CheckDecrease (KEY_T old_key, KEY_T new_key) {};
  static inline void CheckIDRange (int id, int heap_max_size) {};
  static inline void CheckDuplicateID (int id_val) {};
  static inline void CheckId (int id_val) {};
};

// Implementationsschablone einbinden
#define BIN_HEAP_HH__
#include "BinomiHeap.cc"
#undef BIN_HEAP_HH__

#endif BIN_HEAP_HH

