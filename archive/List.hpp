#pragma once

template <typename VALUE, unsigned int SIZE>
class List
{

public:

   struct Entry
   {
      VALUE value;

      bool isFree;

      Entry() : isFree(true)
      {
      }
   };

   List() : containerLength(0)
   {
   }

   void operator=(
      const List& copyObject)
   {
      if (this != &copyObject)
      {
         clear();

         for (int i = 0; i < copyObject.length(); i++)
         {
            add(copyObject.item(i));
         }
      }
   }

   int length()
   {
      return (containerLength);
   }

   bool contains(
      const VALUE& value)
   {
      return (find(value) != sNOT_FOUND);
   }

   Entry* item(
      const int& index)
   {
      Entry* entry = 0;

      if (index < containerLength)
      {
         entry = &(container[index]);
      }

      return (entry);
   }

   bool add(
      const VALUE& value)
   {
      bool isSuccess = false;

      int foundIndex = findFree();

      if (foundIndex != sNOT_FOUND)
      {
         container[foundIndex].value = value;
         container[foundIndex].isFree = false;

         containerLength++;

         isSuccess = true;
      }

      return (isSuccess);
   }

   bool remove(
      const VALUE& value)
   {
      bool isSuccess = false;

      int foundIndex = find(value);

      if (foundIndex != sNOT_FOUND)
      {
         container[foundIndex].isFree = true;

         containerLength--;

         condense();

         isSuccess = true;
      }

      return (isSuccess);
   }

   void clear()
   {
      for (int i = 0; i < SIZE; i++)
      {
         container[i] = Entry();
      }

      containerLength = 0;
   }

private:

   int find(
      const VALUE& value) const
   {
      int foundIndex = sNOT_FOUND;

      for (int i = 0; i < SIZE; i++)
      {
         if ((container[i].isFree == false) &&
             (container[i].value == value))
         {
            foundIndex = i;
            break;
         }
      }

      return (foundIndex);
   }

   int findFree() const
   {
      int foundIndex = sNOT_FOUND;

      for (int i = 0; i < SIZE; i++)
      {
         if (container[i].isFree == true)
         {
            foundIndex = i;
            break;
         }
      }

      return (foundIndex);
   }

   void condense()
   {
      bool done = false;

      for (int i = 0; ((i < SIZE) && (done == false)); i++)
      {
         if (container[i].isFree == true)
         {
            for (int j = (i+1); j < SIZE; j++)
            {
               if (container[j].isFree == false)
               {
                  container[i] = container[j];
                  container[j].isFree = true;
                  break;
               }
               else if (j == (SIZE - 1))
               {
                  done = true;
               }
            }
         }
      }
   }

   static const int sNOT_FOUND;

   Entry container[SIZE];

   int containerLength;
};

template <typename VALUE, unsigned int SIZE>
const int List<VALUE, SIZE>::sNOT_FOUND = SIZE;
