# **LRU Profile Trace**

## 5.1.1
We expect the Ordering in small N to do less deletions and insertions, therefore, it will run faster with single or double probing
so there may not be that big of a difference when double probing. However, when using a large N using a double probe would be
way more efficient since we have more data to cover and using a single probe would take much longer and be less efficient.
We can expect that the small n will have much less clusters then the large n therfore making the graph look dratically different.


## 5.1.2
From what I found fewer probes usually have lower times since that's less probes that they have to do which means a fewer
amount of time running the program. 
I wasn't able to find one that didn't match this description since my terminal memeory had ran out and stopped me from running
more test.

## 5.1.3
In my intemplementation I found that when N is larger you can see a bigger difference with double probing since the amount of 
data that they cover is larger. While having a smaller N means that there will be an extra step with double probing that will
be similar to just using a single probe since there's fewer data to cover. Since I wasn't able to get my csv to work properly
I can't tell the difference, however, I can expect that the smaller n had a better time single probing and larger n had a faster run time double probing.


## 5.1.4
Across N having a larger N it triggered more compactions. This was consistent with it's higher tombstones_pct and its load factor.
Having a higher compactions mean slower run times as after compaction in  N 4096 we see that the runtime after compaction 
went from 20.14 seconds as the mean run length to 5.03 after compaction.

## 5.1.5
Since I was unable to get the csv's to work I believe that if there was a lower latency that would also mean that it had a higher throughput.

## 5.1.6
Im unable to do this one as well :(

## 5.1.7
I was only able to get one before my computer stopped allowing me to capture more data however this is the histogram
![](/Users/anthonyvillalobos/Desktop/untitled folder/HashMapAnalysis_StudentFiles 2/Screenshot 2025-12-04 at 10.12.37 PM.png)
Before compaction the table had long            
Before compaction — summary
Runs counted	236
Bits scanned	5,001
Mean run length	20.14
Median run length	15
p90 run length	42
p95 run length	50
Max run length	136

After compaction — summary
Runs counted	815
Bits scanned	5,101
Mean run length	5.03
Median run length	3
p90 run length	11
p95 run length	14
Max run length	41 

After compaction there was more runs counted and the run length shortened by 15 seconds. 

## Questions to answer
## Where are the longest 1-runs in ACTIVE+DELETED—few big blocks or many medium ones?

I wasn't able to see the graph but I would assume that they occured when having a very large N that had a single probe since
those have very large clusters.

## At the same N, do single and double probing show visibly different clustering?

Yes single probing has more clusters while double probing has clusters that are more evenly spread out.

## Does compaction break up long 1-runs in your “after” snapshot?
Again I cant answer this since I didnt have a proper graph but yes i assume the compaction breaks up the clusters.

## After compaction, do the long runs shrink? Do more runs shift into shorter lengths?
After compaction the long runs get shorter and we get more runs but they are in shorter length. 

## Single vs double at the same N: which has more long runs in ACTIVE+DELETED?
Single has much more long runs but double has more runs that are in shorter length.

## When you see longer runs in ACTIVE+DELETED, do you also see higher average_probes and higher elapsed_ms?
Yes since there is more probes when there is more runs.

## When compaction reduces long runs, do average_probes and elapsed_ms improve?
Yes they also improve

## How do load_factor_pct, eff_load_factor_pct, and tombstones_pct line up with what you see in the maps and histograms?
I wouldnt be able to anwnser this but I assume after compaction the tombsetones and load factor would decrease as average probe times increased. 


