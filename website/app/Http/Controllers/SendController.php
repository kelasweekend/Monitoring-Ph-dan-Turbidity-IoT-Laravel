<?php

namespace App\Http\Controllers;

use App\Models\Sensor;
use Illuminate\Http\Request;

class SendController extends Controller
{
    public function send(Request $request)
    {
        Sensor::create([
            'ph' => $request->ph,
            'turbidity' => $request->turbidity
        ]);

        return 'ok';
    }
}
