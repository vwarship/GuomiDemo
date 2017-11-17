//
//  ViewController.swift
//  GuomiDemo
//
//  Created by 王军建 on 2017/10/26.
//  Copyright © 2017年 狗吃草. All rights reserved.
//

import UIKit

class SM3ViewController: UIViewController, UITextViewDelegate {
    @IBOutlet weak var inTextView: UITextView!
    @IBOutlet weak var outTextView: UITextView!
    
    override func viewDidLoad() {
        super.viewDidLoad()

        inTextView.returnKeyType = .done
        outTextView.returnKeyType = .done
        
        inTextView.delegate = self
        outTextView.delegate = self
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }

    @IBAction func hashButton(_ sender: UIButton) {
        if let inText = inTextView.text {
            let hashCode = CString(size: 100)
            gm_sm3(inText, inText.count, hashCode.toPtr());
            
            outTextView.text = hashCode.toString()
        }
    }
    
    func textView(_ textView: UITextView, shouldChangeTextIn range: NSRange, replacementText text: String) -> Bool {
        if text == "\n" {
            textView.resignFirstResponder()
            return true
        }
        
        return true
    }

}

