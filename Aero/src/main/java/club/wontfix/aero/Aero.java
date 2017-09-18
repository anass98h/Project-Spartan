package club.wontfix.aero;

import lombok.Getter;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import ro.pippo.core.Application;

public class Aero extends Application {

    @Getter private static Aero instance;

    @Getter private Logger logger = LoggerFactory.getLogger(Aero.class);

    @Override
    protected void onInit() {
        instance = this;

        logger.info("Welcome to Aero - The Project Spartan Web Backend.");
    }

    @Override
    protected void onDestroy() {


        logger.info("Thank you and have a nice day.");
        instance = null;
    }

}
