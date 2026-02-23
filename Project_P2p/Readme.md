# Project Peer to Peer (P2P) 
## Yêu cầu.

Xây dựng một ứng dụng chat P2P hoàn chỉnh chạy trên giao diện dòng lệnh (CLI). Thách thức chính là quản lý đồng thời nhiều kết nối TCP, xử lý input từ người
dùng và nhận dữ liệu từ các peer khác mà không làm chương trình bị treo.

**Đề** :*Xây dựng một ứng dụng chat P2P bằng C/C++ (hoặc ngôn ngữ tương đương), cho phép nhiều người dùng kết nối, quản lý kết nối và trao đổi tin nhắn trực tiếp với nhau thông qua các lệnh trên terminal trên Linux*

### 1.1. Lệnh cơ bản và Hiển thị thông tin
• help: Hiển thị danh sách tất cả các lệnh và mô tả ngắn gọn cách sử dụng chúng.

    • myip: In ra địa chỉ IP của máy đang chạy chương trình.
    • myport: In ra cổng (port) mà ứng dụng đang lắng nghe các kết nối đến.
### 1.2. Quản lý Kết nối
    • connect <destination_ip> <port>: Thiết lập một kết nối TCP mới đến một peer khác tại địa chỉ IP và cổng được chỉ định.

    o Xử lý lỗi: Phải kiểm tra và thông báo lỗi cho các trường hợp: IP hoặc  port không hợp lệ, người dùng tự kết nối đến chính mình, hoặc kết nối đã tồn tại.

    • list: Hiển thị danh sách tất cả các kết nối đang hoạt động. Mỗi kết nối phải có một ID duy nhất, địa chỉ IP và cổng của peer.

    • terminate <connection_id>: Ngắt một kết nối cụ thể dựa trên ID của nó trong danh sách list.

    . Xử lý lỗi: Báo lỗi nếu ID không tồn tại.

    • exit: Đóng tất cả các kết nối đang hoạt động một cách an toàn và thoát khỏi chương trình. Các peer khác trong mạng phải nhận biết được sự kiện này và cập nhật danh sách kết nối của họ.
## 1.3. Trao đổi Tin nhắn

    • send <connection_id> <message>: Gửi một tin nhắn đến một peer cụ thể.
        o Giới hạn: Tin nhắn không được dài quá 100 ký tự.
    
    • Nhận tin nhắn: Khi một peer nhận được tin nhắn, nó phải hiển thị thông tin của người gửi (IP, Port) và nội dung tin nhắn.
## 2. Cách biên dịch và demo chương trình

**Biên dịch trên terminal**

*make all -  để build chương trình*

*make clean - xóa file thực thi*

**Run**  

Trên terminal : ./P2P_app < Port >

****Chú ý****: *giá trị port phải nằm trong khoảng cho phép ( từ 1024  đến 49152  )*

*  *Chạy nhiều terminal để test các tính năng*

* *Chỉ connect đồng thời số lượng serve nhất định ( Device = 5)*

ví dụ ./P2P_app 1800