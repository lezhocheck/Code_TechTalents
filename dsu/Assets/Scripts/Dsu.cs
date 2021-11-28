using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Dsu
{
    private readonly Set[] parents;
    private readonly int[] ranks;

    public Dsu(int size)
    {
        parents = new Set[size];
        ranks = new int[size];
    }
    
    public void Make(Set set) 
    {
        parents[set.Id] = set;
        ranks[set.Id] = 0;
    }
 
    public Set Find(Set set) 
    {
        if (set.Id == parents[set.Id].Id)
        {
            return set;    
        }

        parents[set.Id] = Find(parents[set.Id]);
        return parents[set.Id];
    }
 
    public void Union(Set a, Set b) 
    {
        a = Find(a);
        b = Find(b);
        
        if (a.Id != b.Id) 
        {
            if (ranks[a.Id] < ranks[b.Id])
            {
                Swap(ref a, ref b);   
            }
            parents[b.Id] = a;
            if (ranks[a.Id] == ranks[b.Id])
            {
                ++ranks[a.Id];   
            }
        }
    }

    private void Swap(ref Set a, ref Set b)
    {
        Set temp = a;
        a = b;
        b = temp;
    }
}
