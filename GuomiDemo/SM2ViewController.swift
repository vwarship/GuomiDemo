//
//  SM2ViewController.swift
//  GuomiDemo
//
//  Created by 王军建 on 2017/10/27.
//  Copyright © 2017年 狗吃草. All rights reserved.
//

import UIKit

class SM2ViewController: UIViewController {

    override func viewDidLoad() {
        super.viewDidLoad()

        let random = CString(size: 100)
        gm_generate_random(random.toPtr())
        print("random: \(random.toString())")
        
        let text = "SM2 encrypt test"
        
        let publicKey = CString(size: 1000)
        let privateKey = CString(size: 1000)
        gm_sm2_generate_keys(random.toPtr(), publicKey.toPtr(), privateKey.toPtr())
        print("publicKey: \(publicKey.toString())")
        print("privateKey: \(privateKey.toString())")

        let encryptedText = CUString(size: 10000)
        gm_sm2_encrypt(publicKey.toPtr(), text, encryptedText.toPtr())
        print(encryptedText.toString())
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */

}
