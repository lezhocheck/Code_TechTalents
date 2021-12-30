using System;
using System.Collections;
using System.Collections.Generic;

public class Dsu
{
    public class DsuItem
    {
        public Set set;
        public Set parent;
        public int rank;

        public DsuItem(Set set, Set parent, int rank)
        {
            this.set = set;
            this.parent = parent;
            this.rank = rank;
        }
    }

    public List<DsuItem> Items { get; }
    private GameManager manager;

    public Dsu(GameManager manager)
    {
        Items = new List<DsuItem>();
        this.manager = manager;
    }
    
    public void AddNew(Set set)
    {
        DsuItem item = new DsuItem(set, set, 0);
        Items.Add(item);
    }
 
    public Set Find(Set set) 
    {
        if (set.Id == Items[set.Id].parent.Id)
        {
            return set;    
        }
        
        Items[set.Id] = new DsuItem(set, Find(Items[set.Id].parent), Items[set.Id].rank);
        return Items[set.Id].parent;
    }
 
    public void Union(Set a, Set b) 
    {
        a = Find(a);
        b = Find(b);
        if (a.Id != b.Id) 
        {
            if (Items[a.Id].rank < Items[b.Id].rank)
            {
               Swap(ref a, ref b); 
            }
            Items[b.Id] = new DsuItem(Items[b.Id].set, a, Items[b.Id].rank);
            manager.SetsController.AddLine(a, b);
            if (Items[a.Id].rank == Items[b.Id].rank)
            {
                Items[a.Id] = new DsuItem(Items[a.Id].set,
                    Items[a.Id].parent, 
                    Items[a.Id].rank + 1);   
            }
        }
    }

    public Dsu Clone()
    {
        Dsu res = new Dsu(manager);
        
        for(int i = 0; i < Items.Count; i++)
        {
            Set set = Items[i].set.Clone();
            Items[i].set.Hide();
            res.AddNew(set);
        }

        for(int i = 0; i < Items.Count; i++)
        {
            if (Items[i].set.Id != Items[i].parent.Id)
            {
                int setId = Items[i].set.Id;
                int parentId = Items[i].parent.Id;
                if (parentId < setId)
                {
                    res.Union(res.Items[parentId].set, 
                        res.Items[setId].set);                    
                }
                else
                {
                    res.Union(res.Items[setId].set, 
                        res.Items[parentId].set);  
                }

            }
        }

        return res;
    }

    public void Clear()
    {
        foreach (var item in Items)
        {
            item.set.Delete();
        }
        Items.Clear();
    }

    public DsuItem GetItemBySet(Set set)
    {
        return Items[set.Id];
    }

    public bool IsPresenter(Set set)
    {
        DsuItem item = GetItemBySet(set);
        return set.Id == item.parent.Id;
    }
    
    private void Swap(ref Set lSet, ref Set rSet)
    {
        Set temp = lSet;
        lSet = rSet;
        rSet = temp;
    }
}
