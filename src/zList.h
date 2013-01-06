#if !defined(ZLIST_H_INCLUDED)
#define ZLIST_H_INCLUDED

template <class T>
class zList  
{
public:
   
   //----------------------- SOTTO CLASSE zNode -----------------------
   //template <class T>
   class zNode  
   {
   public:
   	zNode(zList *l)
      {
         MyList=l;
      }

	   ~zNode()
      {
      }

  	   zNode *insert(T *elem)	// Inserisci nuovo
	   {
		   zNode *temp = new zNode(MyList);
         temp->Prev=Prev;
         temp->Next=this;
         Prev->Next=temp;
         Prev=temp;
         temp->Elem=elem;
         return temp;
	   }

      bool isEnd()			// Fine lista?
	   {
         return (this==MyList->sentinel);
	   }

      void remove()	
	   {
         Prev->Next=Next;
         Next->Prev=Prev;
         delete this;
	   }

		void moveToBack()
		{
			//devo sganciarmi dalla posizione corrente
			Prev->Next=Next;
         Next->Prev=Prev;

			// devo riagganciarmi in fondo
			Prev=MyList->sentinel->Prev;
			MyList->sentinel->Prev->Next=this;
			MyList->sentinel->Prev=this;
			Next=MyList->sentinel;
		}

      T *Elem;

      zList *MyList;
  	   zNode *Next;
      zNode *Prev;
   };


	zList()					// COSTRUTTORE 1
	{
      sentinel=new zNode(this);
      sentinel->Prev=sentinel;
      sentinel->Next=sentinel;
	}
	

	~zList()				// DISTRUTTORE
	{
		destroy();
	}
	
	zNode *begin()			// Primo lista
	{
      return sentinel->Next;
   }

	zNode *end()			// Ultimo lista
	{
      return sentinel->Prev;
   }
	
	bool isEmpty()			// Lista vuota?
	{
      return (sentinel->Next==sentinel);
	}

   zNode *insertFront(T *e)
   {
      return sentinel->Next->insert(e);
   }

   zNode *insertBack(T *e)
   {
      return sentinel->insert(e);
   }

	void destroy()
	{
		zNode *node = first();
		while(!isEmpty())
		{
			zNode *next = node->Next;
			node->remove();
			node=next;
		}
		delete sentinel;
	}

//protected:

   zNode *sentinel;
   
};

#endif // !defined(zList_H_INCLUDED)

