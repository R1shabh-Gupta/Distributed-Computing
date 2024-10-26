import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class RMIServer {
    public static void main(String[] args) {
        try {
            // Create and export the remote object
            RemoteInterface remoteObj = new RemoteImpl();
            
            // Create and start the registry on port 1099
            Registry registry = LocateRegistry.createRegistry(1999);
            
            // Bind the remote object to the registry
            registry.bind("RemoteService", remoteObj);
            
            System.out.println("Server is ready!");
        } catch (Exception e) {
            System.err.println("Server exception: " + e.toString());
            e.printStackTrace();
        }
    }
}
