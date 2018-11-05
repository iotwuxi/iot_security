package org.iotwuxi.embedded;

import org.eclipse.californium.core.CoapClient;
import org.eclipse.californium.core.CoapResponse;
import org.eclipse.californium.core.Utils;
import org.eclipse.californium.core.network.CoapEndpoint;
import org.eclipse.californium.core.network.config.NetworkConfig;
import org.eclipse.californium.scandium.DTLSConnector;
import org.eclipse.californium.scandium.ScandiumLogger;
import org.eclipse.californium.scandium.config.DtlsConnectorConfig;
import org.eclipse.californium.scandium.dtls.pskstore.StaticPskStore;

import java.io.IOException;
import java.io.InputStream;
import java.net.InetSocketAddress;
import java.net.URI;
import java.net.URISyntaxException;
import java.security.GeneralSecurityException;
import java.security.KeyStore;
import java.security.PrivateKey;
import java.security.cert.Certificate;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;

public class App
{
    static {
        ScandiumLogger.initialize();
        ScandiumLogger.setLevel(Level.FINE);
    }

    private static final String LOCAL_SERVER_URI = "coaps://localhost/time";

    private DTLSConnector dtlsConnector;

    public App() {

        DtlsConnectorConfig.Builder builder = new DtlsConnectorConfig.Builder(new InetSocketAddress(0));
        builder.setPskStore(new StaticPskStore("identity", "password".getBytes()));

        dtlsConnector = new DTLSConnector(builder.build());
    }

    public void test(String[] args) {

        CoapResponse response = null;
        URI uri;
        try {
            if (args.length > 0) {
                uri =  new URI(args[0]);
            } else {
                uri = new URI(LOCAL_SERVER_URI);
            }
            System.out.println("URI: " + uri.toString());

            CoapClient client = new CoapClient(uri);
            client.setEndpoint(new CoapEndpoint(dtlsConnector, NetworkConfig.getStandard()));
            response = client.get();

        } catch (URISyntaxException e) {
            System.err.println("Invalid URI: " + e.getMessage());
            System.exit(-1);
        }

        if (response != null) {

            System.out.println(response.getCode());
            System.out.println(response.getOptions());
            System.out.println(response.getResponseText());

            System.out.println("\nADVANCED\n");
            System.out.println(Utils.prettyPrint(response));

        } else {
            System.out.println("No response received.");
        }
    }

    public static void main(String[] args) throws InterruptedException {

        App client = new App();
        client.test(args);

        synchronized (App.class) {
            App.class.wait();
        }
    }
}
