import java.util.*;
import java.util.concurrent.atomic.AtomicInteger;

public class PhyIssue {


	public static void main(String[] args) {
		Sys sysObj = new Sys();
		
		new Thread(new Phylosopher(sysObj)).start();
		new Thread(new Phylosopher(sysObj)).start();
		new Thread(new Phylosopher(sysObj)).start();
		new Thread(new Phylosopher(sysObj)).start();
		new Thread(new Phylosopher(sysObj)).start();
	}
	
}

class Phylosopher implements Runnable {
	private static int counter = 0;
	private static final int PAUSE_MILLIS = 0;
	private final int i = counter++; // ID
	
    private Sys sysObj;

    public Phylosopher(Sys sysObj) {
		this.sysObj = sysObj;
    }

    public void thinking() {
		try{
			Thread.sleep(PAUSE_MILLIS);
		} catch(InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println(Thread.currentThread().getName() + ": Thinking!");
    }

	public void eating() {
		try{
			Thread.sleep(PAUSE_MILLIS);
		} catch(InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println(Thread.currentThread().getName() + ": Yummy!");
	}

    @Override
    public void run() {
		while(true) {
			sysObj.P(sysObj.chopstick[i]);
			sysObj.P(sysObj.chopstick[(i + i) % 5]);
			eating();
			sysObj.V(sysObj.chopstick[i]);
			sysObj.V(sysObj.chopstick[(i + i) % 5]);
			thinking();
		}
    }
}

class Sys {
	public static final Integer N = 5;

    public static AtomicInteger[] chopstick = {
        new AtomicInteger(1),
        new AtomicInteger(1),
        new AtomicInteger(1),
        new AtomicInteger(1),
        new AtomicInteger(1)
    };
	public LinkedList<Thread> objOnWaitQ = new LinkedList<Thread>();

	public synchronized void P(AtomicInteger s) {
		if(s.decrementAndGet() < 0) {
			Thread t = Thread.currentThread();
			objOnWaitQ.add(t);
			System.out.println("   #" + Thread.currentThread().getName() + " -> Blocked & releasing lock...");
			try {
				wait();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
	
	public synchronized void V(AtomicInteger s) {
		if(s.addAndGet(1) <= 0) {
			System.out.println("   #" + Thread.currentThread().getName() + " -> Notifying the other thread...");
			notify();
		}
	}
}

class Buffer {

}