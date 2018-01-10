#pragma once

template <typename KEY, typename VALUE, unsigned int SIZE>
class Map
{

public:

   struct Entry
   {
      KEY key;

      VALUE value;

      bool isFree;

      Entry() : isFree(true)
      {
      }
   };

   Map() : containerLength(0)
   {
   }

   bool isSet(
      const KEY& key) const
   {
      return (findKey(key) != sNOT_FOUND);
   }

   int length() const
   {
      return (containerLength);
   }

   const Entry* find(
      const VALUE& value)
   {
      Entry* entry = 0;

      int foundIndex = findValue(value);

      if (foundIndex != sNOT_FOUND)
      {
         entry = &(container[foundIndex]);
      }

      return (entry);
   }

   const Entry* item(
      const int& index) const
   {
      const Entry* entry = 0;

      if (index < containerLength)
      {
         entry = &(container[index]);
      }

      return (entry);
   }

   const VALUE* get(
      const KEY& key) const
   {
      const VALUE* value = 0;

      int foundIndex = findKey(key);

      if (foundIndex != sNOT_FOUND)
      {
         value = &(container[foundIndex].value);
      }

      return (value);
   }

   const Entry* put(
      const KEY& key,
      const VALUE& value)
   {
      const Entry* entry;

      int foundIndex = findKey(key);

      if (foundIndex != sNOT_FOUND)
      {
         container[foundIndex].value = value;

         entry = &(container[foundIndex]);
      }
      else
      {
         foundIndex = findFree();

         if (foundIndex != sNOT_FOUND)
         {
            container[foundIndex].key = key;
            container[foundIndex].value = value;
            container[foundIndex].isFree = false;

            containerLength++;

            entry = &(container[foundIndex]);
         }
      }

      return (entry);
   }

   bool erase(
      const KEY& key)
   {
      bool isSuccess = false;

      int foundIndex = findKey(key);

      if (foundIndex != sNOT_FOUND)
      {
         container[foundIndex].isFree = true;

         containerLength--;

         condense();

         isSuccess = true;
      }

      return (isSuccess);
   }

   VALUE& operator[](
      const KEY& key)
   {
      int foundIndex = findKey(key);

      if (foundIndex != sNOT_FOUND)
      {
         return (container[foundIndex].value);
      }
      else
      {
         foundIndex = findFree();

         if (foundIndex != sNOT_FOUND)
         {
            container[foundIndex].key = key;
            container[foundIndex].isFree = false;

            containerLength++;

            return (container[findKey(key)].value);
         }
         else
         {
            // Subscript out of bounds.
            // Return the first entry in the array.
            return (container[0].value);
         }
      }
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

   int findKey(
      const KEY& key) const
   {
      int foundIndex = sNOT_FOUND;

      for (int i = 0; i < SIZE; i++)
      {
         if ((container[i].isFree == false) &&
             (container[i].key == key))
         {
            foundIndex = i;
            break;
         }
      }

      return (foundIndex);
   }

   int findValue(
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

template <typename KEY, typename VALUE, unsigned int SIZE>
const int Map<KEY, VALUE, SIZE>::sNOT_FOUND = SIZE;
