package org.iotwuxi.embedded;

import org.eclipse.californium.elements.AddressEndpointContext;
import org.eclipse.californium.elements.RawData;
import org.eclipse.californium.elements.RawDataChannel;
import org.eclipse.californium.scandium.DTLSConnector;
import org.eclipse.californium.scandium.config.DtlsConnectorConfig;
import org.eclipse.californium.scandium.dtls.pskstore.StaticPskStore;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;


public class DTLSClient
{

    private static final int DEFAULT_PORT = 4433;
    private static final Logger LOG = LoggerFactory.getLogger(DTLSClient.class.getName());

    private DTLSConnector dtlsConnector;

    public DTLSClient(final CountDownLatch latch) {
        DtlsConnectorConfig.Builder builder = new DtlsConnectorConfig.Builder();
        builder.setPskStore(new StaticPskStore("Client_identity", "secretPSK".getBytes()));

        dtlsConnector = new DTLSConnector(builder.build());
        dtlsConnector.setRawDataReceiver(new RawDataChannel() {

            @Override
            public void receiveData(RawData raw) {
                LOG.info("Received response: {}", new String(raw.getBytes()));
                latch.countDown();
                dtlsConnector.destroy();
            }
        });
    }

    private void sendHelloMessage(InetSocketAddress peer) {
        try {
            dtlsConnector.start();
            dtlsConnector.send(RawData.outbound("HELLO WORLD".getBytes(), new AddressEndpointContext(peer), null, false));
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) throws InterruptedException {

        final CountDownLatch latch = new CountDownLatch(1);
        DTLSClient client = new DTLSClient(latch);
        InetSocketAddress peer = new InetSocketAddress("localhost", DEFAULT_PORT);
        if (args.length == 2) {
            peer = new InetSocketAddress(args[0], Integer.parseInt(args[1]));
        }
        client.sendHelloMessage(peer);
        latch.await(5, TimeUnit.SECONDS);
    }
}
