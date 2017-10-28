//
//  CharacterConversionToolViewController.swift
//  GuomiDemo
//
//  Created by 王军建 on 2017/10/28.
//  Copyright © 2017年 狗吃草. All rights reserved.
//

import UIKit

class CharacterConversionToolViewController: UIViewController {
    @IBOutlet weak var inTextView: UITextView!
    @IBOutlet weak var outTextView: UITextView!
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
    }
    
    @IBAction func buffer2HexstrButton(_ sender: UIButton) {
        if let inText = inTextView.text {
            let len = inText.lengthOfBytes(using: .ascii)
            let hexstr = CString(size: len * 3)
            gm_buffer2hexstr(inText, len, hexstr.toPtr())
            
            outTextView.text = hexstr.toString()
        }
    }
    
    @IBAction func hexstr2BufferButton(_ sender: UIButton) {
        if let inText = inTextView.text {
            let len = inText.lengthOfBytes(using: .ascii)
            let buffer = CUString(size: len)
            var bufferLength: Int = 0
            let bufferLengthPtr = UnsafeMutablePointer(&bufferLength)
            gm_hexstr2buffer(inText, buffer.toPtr(), bufferLengthPtr)

            outTextView.text = buffer.toString()
        }
    }
    
    @IBAction func reserverInAndOutText(_ sender: UIButton) {
        let inText = inTextView.text ?? ""
        let outText = outTextView.text ?? ""
        
        inTextView.text = outText
        outTextView.text = inText
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
