
import threading

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
                            