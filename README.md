# Automated Conference Scheduler

A  conference has  n  papers  accepted.  Our  job  is  to  organize  them  in  a  best  possible  schedule. 
The schedule has p parallel sessions at a given time. Each session has k papers. And there are a total of t time slots. 
We  first  define  the  characteristics  of  a  good  schedule.  For  any  good  schedule  most  people 
should  feel  no  conflict  about which  session  to  attend.  That  is,  (1)  all  papers  in  one  session 
should be related to a single theme. And (2) all papers in parallel sessions should be as far away  as possible to avoid conflict. More details in the doc.

We used simulated annealing with random restarts to achieve the best state.
