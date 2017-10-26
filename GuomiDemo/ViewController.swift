//
//  ViewController.swift
//  GuomiDemo
//
//  Created by 王军建 on 2017/10/26.
//  Copyright © 2017年 狗吃草. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    @IBOutlet weak var inTextView: UITextView!
    @IBOutlet weak var outTextView: UITextView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
////        let str = "34523D2F917420E702DBA970C071AE4971AD08DE3D7D0D90DC1E334ED20444E54F109BA80DD22F25C24FAA83D5AD58687F1AA68F1B749D0AD999DB9A1AC8E4DC"
////        let str = "616263"
//        let str = "abc"
//        let md5Count = 100
//        let md5Ptr = UnsafeMutablePointer<Int8>.allocate(capacity: md5Count)
//        gm_md5(str, str.count, md5Ptr);
//        let md5 = String.init(cString: md5Ptr)
//        print("md5: \(md5)")
//        md5Ptr.deallocate(capacity: md5Count)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }

    @IBAction func hashButton(_ sender: UIButton) {
        if let inText = inTextView.text {
            let md5Count = 100
            let md5Ptr = UnsafeMutablePointer<Int8>.allocate(capacity: md5Count)
            gm_md5(inText, inText.count, md5Ptr);
            let md5 = String.init(cString: md5Ptr)
            md5Ptr.deallocate(capacity: md5Count)
            
            outTextView.text = md5
        }
    }
}

