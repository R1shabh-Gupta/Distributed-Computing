import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class RMIClient {
    public static void main(String[] args) {
        try {
            // Get the registry
            Registry registry = LocateRegistry.getRegistry("localhost", 1999);
            
            // Look up the remote object
            RemoteInterface remoteObj = (RemoteInterface) registry.lookup("RemoteService");
            
            // Call remote methods
            String response = remoteObj.sayHello("Student");
            System.out.println("Response from server: " + response);
            
            int sum = remoteObj.add(5, 3);
            System.out.println("5 + 3 = " + sum);
            
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
