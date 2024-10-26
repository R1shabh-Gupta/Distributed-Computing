import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RemoteInterface extends Remote {
    String sayHello(String name) throws RemoteException;
    int add(int a, int b) throws RemoteException;
}