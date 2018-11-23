<?php

namespace App\Controllers;

use Psr\Http\Message\RequestInterface;
use Psr\Http\Message\ResponseInterface;

class HomeCtrl extends Controller {
    public function Home($request, $response) {
        $this->render($response, "Home.twig");
    }

    public function Lights($request, $response, $args) {
        if ($args["state"] == "on") {
            shell_exec("sudo python ../Python/Script.py light on");
        } else {
            shell_exec("sudo python ../Python/Script.py light off");
        }
    }
}
