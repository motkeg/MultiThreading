import threading
import time



threadLock = threading.Lock()
#global search tread
class SearchThread (threading.Thread):
    def __init__(self, threadID, name, path ,w_list):
        threading.Thread.__init__(self)
        self.threadID = threadID
        self.name = name
        self.path = path
        self.list=w_list
    def run(self):
        print "Starting " + self.name +"-"+ time.ctime(time.time())
        # Get lock to synchronize threads
        threadLock.acquire()
        search(self.path,self.list )
        # Free lock to release next thread
        threadLock.release()
        
 
 #thread for every word       
class word_SearchThread (threading.Thread):
    def __init__(self,path ,w):
        threading.Thread.__init__(self)
        self.path = path
        self.list=w
    def run(self):
        #print "Starting " + self.name +"-"+ time.ctime(time.time())
        # Get lock to synchronize threads
        #threadLock.acquire()
        w_search(self.path,self.list )
        # Free lock to release next thread
        #threadLock.release()      

##-------------------------------------------------------------------   
def search(path,w_search):
    try:
       
        for w in w_search:
            t=word_SearchThread(path,w)
            t.start()
           
    except ValueError,IOError:
        print "file could not open!"
                    
##-------------------------------------------------------
def w_search(path,w):
    try:
        file=open(path,"r")
        i=1
        for line in file:
            if w in line:
                print "{0}/[{3}] ({1}): {2}".format(file.name,i,line,w)
            i+=1
    except ValueError,IOError:
        print "file could not open!"
                    

##########################################################################

words1=["developing","experience" ,"multi"]# raw_input("enter list of words to search FILE 1 (use ' ' between):").split()#

words2=["SQL","C++" ,"java","thread"]# raw_input("enter list of words to search FILE 2 (use ' ' between):").split()#
#init threads    
threads=SearchThread(1,"A","cv1.txt",words1),SearchThread(2,"B","cv2.txt",words2)

print "------------------------------------START-*-------------------"
threads[0].start()
threads[1].start()
###- wait for and all threads
for t in threads:
    t.join()
print "-------------------------Exiting Main Thread----------------------"
           
                
        
         