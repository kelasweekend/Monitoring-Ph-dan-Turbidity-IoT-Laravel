<?php

namespace App\Http\Controllers;

use App\Models\Sensor;
use Illuminate\Http\Request;

class HomeController extends Controller
{
    public function index()
    {
        return view('welcome');
    }

    public function dataSensor(Request $request)
    {
        $data = Sensor::latest()->first();
        return response()->json([
            'success' => true,
            'data' => [
                'ph' => $data->ph,
                'ntu' => $data->turbidity
            ]
        ], 200);
    }
}
