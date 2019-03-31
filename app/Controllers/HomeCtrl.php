<?php

namespace App\Controllers;

use Psr\Http\Message\RequestInterface;
use Psr\Http\Message\ResponseInterface;

class HomeCtrl extends Controller {
    public function Home($request, $response) {
        $this->render($response, "Home.twig");
    }

    public function Lights($request, $response, $args) { // deviceID=1&cmd=LAMP_ON
        var_dump($args);
        shell_exec("sudo python ../Python/Tester.py \"deviceID=" . $args['deviceID'] . "&cmd=" . $args['command'] . "\"");
        sleep(1);
        shell_exec("sudo python ../Python/Tester.py \"deviceID=" . $args['deviceID'] . "&cmd=" . $args['command'] . "\"");
    }

    public function Computer($request, $response, $args) {
    	shell_exec("sudo etherwake -i wlan0 " . $args['cmd'] . "");
    }
}
