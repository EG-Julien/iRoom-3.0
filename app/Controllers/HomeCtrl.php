<?php

namespace App\Controllers;

use Psr\Http\Message\RequestInterface;
use Psr\Http\Message\ResponseInterface;

class HomeCtrl extends Controller {
    public function Home($request, $response) {
        $this->render($response, "Home.twig");
    }

    public function Lights($request, $response, $args) {
        shell_exec("sudo python ../Python/Script.py " . $args['cmd'] . "");
    }

    public function Computer($request, $response, $args) {
    	shell_exec("sudo etherwake -i wlan0 " . $args['cmd'] . "");
    }
}
