//
//  ViewController.swift
//  GuomiDemo
//
//  Created by 王军建 on 2017/10/26.
//  Copyright © 2017年 狗吃草. All rights reserved.
//

import UIKit

class SM3ViewController: UIViewController {
    @IBOutlet weak var inTextView: UITextView!
    @IBOutlet weak var outTextView: UITextView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }

    @IBAction func hashButton(_ sender: UIButton) {
        if let inText = inTextView.text {
            let md5 = CString(size: 100)
            gm_md5(inText, inText.count, md5.toPtr());
            
            outTextView.text = md5.toString()
        }
    }
}

